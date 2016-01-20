/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2002 PDFlib GmbH and Thomas Merz. All rights reserved. |
 +---------------------------------------------------------------------------+
 |    This software is NOT in the public domain.  It can be used under two   |
 |    substantially different licensing terms:                               |
 |                                                                           |
 |    The commercial license is available for a fee, and allows you to       |
 |    - ship a commercial product based on PDFlib                            |
 |    - implement commercial Web services with PDFlib                        |
 |    - distribute (free or commercial) software when the source code is     |
 |      not made available                                                   |
 |    Details can be found in the file PDFlib-license.pdf.                   |
 |                                                                           |
 |    The "Aladdin Free Public License" doesn't require any license fee,     |
 |    and allows you to                                                      |
 |    - develop and distribute PDFlib-based software for which the complete  |
 |      source code is made available                                        |
 |    - redistribute PDFlib non-commercially under certain conditions        |
 |    - redistribute PDFlib on digital media for a fee if the complete       |
 |      contents of the media are freely redistributable                     |
 |    Details can be found in the file aladdin-license.pdf.                  |
 |                                                                           |
 |    These conditions extend to ports to other programming languages.       |
 |    PDFlib is distributed with no warranty of any kind. Commercial users,  |
 |    however, will receive warranty and support statements in writing.      |
 *---------------------------------------------------------------------------*/

/* $Id: p_font.c,v 1.42.10.15 2002/01/26 18:43:19 tm Exp $
 *
 * PDFlib font handling routines
 *
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "p_intern.h"
#include "p_font.h"
#include "p_cid.h"
#include "p_encoding.h"

/*
 * Include files with more built-in font metrics can easily be added here.
 */

const pdf_font pdf_core_metrics[] = {

#include "p_metrics.h"

};

#define SIZEOF_CORE_METRICS	\
	((int) (sizeof(pdf_core_metrics)/sizeof(pdf_font)))

/* 
 * PDF's base 14 fonts, guaranteed to be always available.
 * Note that this list is not necessarily identical to the list of font
 * names in pdf_core_fonts which is essentially only a cache of font
 * metrics.
 */
static const char *pdf_base14_names[] = {
  "Courier", "Courier-Bold", "Courier-Oblique", "Courier-BoldOblique",
  "Helvetica", "Helvetica-Bold", "Helvetica-Oblique", "Helvetica-BoldOblique",
  "Symbol",
  "Times-Roman", "Times-Bold", "Times-Italic", "Times-BoldItalic",
  "ZapfDingbats",
  NULL
};

/* Type 1 font portions: ASCII, encrypted, zeros */
typedef enum { ascii, encrypted, zeros } pdf_t1portion;

typedef struct {
    pdf_t1portion portion;
    size_t length1, length2, length3;
    FILE *fontfile;
} t1_private_data;

const char *
pdf_get_fontname(PDF *p)
{
    if (p->fonts_number == 0 || p->tstate[p->sl].f == -1) /* no font set */
	pdf_error(p, PDF_RuntimeError,
	    "Queried current font name before setting font");

    return p->fonts[p->tstate[p->sl].f].name;
}

const char *
pdf_get_fontencoding(PDF *p)
{
    pdf_font *font;
    char *ret;

    font = &p->fonts[p->tstate[p->sl].f];

    if (PDF_SANITY_CHECK_FAILED(p))
	return ((const char *) NULL);

    if (p->fonts_number == 0 || p->tstate[p->sl].f == -1) /* no font set */
	pdf_error(p, PDF_RuntimeError,
	    "Queried current encoding before setting font");

    switch (font->encoding) {
	case builtin:
	    ret = "builtin";
	    break;

	case cid:
	    ret = font->encodingScheme;
	    break;

	default:
	    ret = p->encodings[font->encoding]->apiname;
    }

    return ret;
}

int
pdf_get_font(PDF *p)
{
    if (PDF_SANITY_CHECK_FAILED(p))
	return 0;

    if (p->fonts_number == 0 || p->tstate[p->sl].f == -1) { /* no font set */
	pdf_error(p, PDF_RuntimeError,
	    "PDF_get_value: queried font before setting one");
    }
    return p->tstate[p->sl].f;
}

float
pdf_get_fontsize(PDF *p)
{
    if (p->fonts_number == 0 || p->tstate[p->sl].f == -1) { /* no font set */
	pdf_error(p, PDF_RuntimeError,
	    "PDF_get_value: queried font size before setting a font");
    }

    return p->tstate[p->sl].fs;
}

void
pdf_cleanup_font_struct(PDF *p, pdf_font *font)
{
    if (font == NULL)
	return;

    if (font->img)
	p->free(p, font->img);

    if (font->name)
	p->free(p, font->name);

    if (font->ttname)
	p->free(p, font->ttname);

    if (font->encodingScheme)
	p->free(p, font->encodingScheme);

    if (font->cmi != NULL) {
	int i;
	CharMetricInfo *temp = font->cmi;
	Ligature *node, *node1;
	
	for (i = 0; i < font->numOfChars; ++i) {
	    for (node = temp->ligs; node != NULL; /* */) {
		p->free(p, node->succ);
		p->free(p, node->lig);
		node1 = node->next;
		p->free(p, node);
		node = node1;
	    }
	    p->free(p, temp->name);
	    temp++;
	}
	
	p->free(p, font->cmi);
    }

    if (font->tkd != NULL)
	p->free(p, font->tkd);

    if (font->pkd != NULL) {
	p->free(p, font->pkd->name1);
	p->free(p, font->pkd->name2);
	p->free(p, font->pkd);
    }

    if (font->ccd != NULL) {
	int i, j;
	CompCharData *ccd = font->ccd;
	
	for (i = 0; i < font->numOfComps; ++i) {
	    for (j = 0; j < ccd[i].numOfPieces; ++j)
		p->free(p, ccd[i].pieces[j].pccName); 

	    p->free(p, ccd[i].ccName);
	}
	p->free(p, font->ccd);
    }
}

void
pdf_cleanup_enc(PDF *p, pdf_encodingvector *enc)
{
    int slot;

    p->free(p, enc->apiname);

    if (enc->codes)
	p->free(p, enc->codes);

    for (slot = 0; slot < 256; slot++)
	if (enc->chars[slot])
	    p->free(p, enc->chars[slot]);

    p->free(p, enc);
}

void
pdf_cleanup_fonts(PDF *p)
{
    int slot;

    if (!p->fonts)
	return;

    for (slot = 0; slot < p->fonts_number; slot++)
	pdf_cleanup_font_struct(p, &p->fonts[slot]);

    /* 
     * p->fontfilename was malloc'ed from the resource machinery
     * and only referenced in the font struct.
     */

    if (p->fonts)
	p->free(p, p->fonts);

    /* clean up user-defined encodings */
    for (slot = SIZEOF_CORE_ENCODINGS; slot < ENC_MAX; slot++) {
	if (p->encodings[slot]) {
	    pdf_cleanup_enc(p, p->encodings[slot]);
	    p->encodings[slot] = NULL;
	}
    }

    p->fonts = NULL;
}

void
pdf_init_fonts(PDF *p)
{
    int slot;

    p->fonts_number     = 0;
    p->fonts_capacity	= FONTS_CHUNKSIZE;

    p->fonts = (pdf_font *) p->malloc(p,
    		sizeof(pdf_font) * p->fonts_capacity, "PDF_init_fonts");

    /* Unfortunately, sizeof can't be used in #if conditionals */
    if (SIZEOF_CORE_ENCODINGS > ENC_MAX)
	pdf_error(p, PDF_SystemError,
	    "Too many built-in encodings; increase ENC_MAX in p_intern.h!");

    /* initialize font encodings */
    for (slot = 0; slot < SIZEOF_CORE_ENCODINGS; slot++)
	p->encodings[slot] = pdf_core_encodings[slot];

    for (slot = SIZEOF_CORE_ENCODINGS; slot < ENC_MAX; slot++)
	p->encodings[slot] = NULL;
}

static void
pdf_grow_fonts(PDF *p)
{
    p->fonts = (pdf_font *) p->realloc(p, p->fonts,
		sizeof(pdf_font) * 2 * p->fonts_capacity, "pdf_grow_fonts");

    p->fonts_capacity *= 2;
}

void
pdf_write_page_fonts(PDF *p)
{
    int i, total = 0;

    for (i = 0; i < p->fonts_number; i++)
	if (p->fonts[i].used_on_current_page == pdf_true)
	    total++;

    if (total > 0) {
	pdf_puts(p, "/Font");

	pdf_begin_dict(p);		/* font resource dict */

	for (i = 0; i < p->fonts_number; i++)
	    if (p->fonts[i].used_on_current_page == pdf_true) {
		p->fonts[i].used_on_current_page = pdf_false;	/* reset */
		pdf_printf(p, "/F%d %ld 0 R\n", i, p->fonts[i].obj_id);
	    }

	pdf_end_dict(p);		/* font resource dict */
    }
}

static pdf_bool
pdf_get_metrics_core(PDF *p, pdf_font *font, const char *fontname, int enc)
{
    int slot;

    (void) p;	/* avoid compiler warning "unreferenced parameter" */

    for (slot = 0; slot < SIZEOF_CORE_METRICS; slot++) {
	if (pdf_core_metrics[slot].encoding == enc &&
	    !strcmp(pdf_core_metrics[slot].name, fontname)) {
		memcpy(font, &pdf_core_metrics[slot], sizeof(pdf_font));

		font->name = pdf_strdup(p, pdf_core_metrics[slot].name);

		if (pdf_core_metrics[slot].encodingScheme)
		    font->encodingScheme =
			pdf_strdup(p, pdf_core_metrics[slot].encodingScheme);

		if (pdf_core_metrics[slot].ttname)
		    font->ttname = pdf_strdup(p, pdf_core_metrics[slot].ttname);

		if (pdf_core_metrics[slot].fontfilename)
		    font->ttname =
			pdf_strdup(p, pdf_core_metrics[slot].fontfilename);

		return pdf_true;
	}
    }

    return pdf_false;
}

static pdf_bool
pdf_get_metrics_cid(
    PDF *p,
    pdf_font *font,
    const char *fontname,
    const char *enc)
{
    int slot, cmap;

    /* Check the requested CMap */
    for (cmap = 0; cmap < NUMBER_OF_CMAPS; cmap++)
	if (!strcmp(cmaps[cmap].name, enc))
	    break;

    /* Unknown CMap */
    if (cmap == NUMBER_OF_CMAPS)
	return pdf_false;

    /* Check whether this CMap is supported in the desired PDF version */
    if (cmaps[cmap].compatibility > p->compatibility) {
	pdf_error(p, PDF_RuntimeError,
	    "CMap %s is not supported in PDF %d.%d", enc,
	    p->compatibility/10, p->compatibility % 10);
    }

    /* Check whether the font name is among the known CID fonts */
    for (slot = 0; slot < SIZEOF_CID_METRICS; slot++) {
	if (!strcmp(pdf_cid_metrics[slot].name, fontname))
	    break;
    }

    /* Unknown font */
    if (slot == SIZEOF_CID_METRICS)
	pdf_error(p, PDF_ValueError, "Unknown CID font '%s'", fontname);

    /* Selected CMap and font don't match */
    if (cmaps[cmap].charcoll != cc_identity &&
	cmaps[cmap].charcoll != pdf_cid_metrics[slot].charcoll)
	pdf_error(p, PDF_ValueError, "Can't use encoding '%s' with font '%s'",
		enc, fontname);

    memcpy(font, &pdf_cid_metrics[slot], sizeof(pdf_font));
    return pdf_true;
}

static int
pdf_handle_cidfont(PDF *p, const char *fontname, const char *encoding)
{
    int slot;

    /*
     * Look whether font is already in the cache.
     * If font with same encoding and embedding flag is found,
     * return its descriptor.
     */

    for (slot = 0; slot < p->fonts_number; slot++) {
	if (p->fonts[slot].encoding == cid &&
	    !strcmp(p->fonts[slot].name, fontname) &&
	    !strcmp(p->fonts[slot].encodingScheme, encoding))
	    return slot;
    }
    
    if (slot >= p->fonts_capacity)
	pdf_grow_fonts(p);

    /* Check for known CID font name and fetch font descriptor data */
    if (!pdf_get_metrics_cid(p, &p->fonts[slot], fontname, encoding))
	return -1;
	
    /* Now everything is fine; fill the remaining entries */
    p->fonts[slot].encodingScheme	= pdf_strdup(p, encoding);
    p->fonts[slot].name			= pdf_strdup(p, fontname);
    p->fonts[slot].obj_id		= pdf_alloc_id(p);
    p->fonts[slot].encoding		= cid;
    p->fonts[slot].embed		= pdf_false;

    p->fonts_number++;

    /* return valid font descriptor */
    return slot;
}

PDFLIB_API int PDFLIB_CALL
PDF_findfont(PDF *p, const char *fontname, const char *encoding, int embed)
{
    static const char fn[] = "PDF_findfont";
    int slot;
    int enc;
    char *filename = NULL;
    char *mmparam, mastername[64];

    PDF_TRACE(("%s\t(pdf[%p], \"%s\", \"%s\", %d);",
	fn, (void *) p, fontname, encoding, embed));

    if (PDF_SANITY_CHECK_FAILED(p))
	return -1;

    PDF_CHECK_SCOPE(p, fn, pdf_state_document | pdf_state_ppt);

    if (fontname == NULL || *fontname == '\0')
	pdf_error(p, PDF_ValueError, "Null fontname");

    if (encoding == NULL || *encoding == '\0')
	pdf_error(p, PDF_ValueError, "Null encoding for font '%s'", fontname);

    if (embed < 0 || embed > 1)
	pdf_error(p, PDF_ValueError,
	    "PDF_findfont: bad embedding code %d", embed);

    if (!strcmp(encoding, "builtin")) {
	enc = builtin;

    } else {
	if (!strcmp(encoding, "host")) {
	/* special name for the platform-specific host encoding */
#if defined(PDFLIB_EBCDIC)
	encoding = "ebcdic";
#elif defined(MAC)
	encoding = "macroman";
#else /* !PDFLIB_EBCDIC && !MAC */
	encoding = "winansi";
#endif /* !PDFLIB_EBCDIC && !MAC */
	}

	/* search for a registered encoding */
	for (enc = 0; p->encodings[enc] != NULL; enc++) {
	    if (p->encodings[enc]->apiname &&
		!strcmp(encoding, p->encodings[enc]->apiname))
		break;
	}
    }

    /* not a known encoding */
    if (enc != builtin && p->encodings[enc] == NULL) {
	/* check the known CMap names */
	if ((slot = pdf_handle_cidfont(p, fontname, encoding)) != -1) {
	    if (p->compatibility == PDF_1_2) {
		if (p->debug['F'] == pdf_true) {
		    pdf_error(p, PDF_RuntimeError,
			"CID fonts are not fully supported in PDF 1.2");
		} else {
		    PDF_TRACE((" [%d]\n", -1));
		    return -1;
		}
	    } else {
		PDF_TRACE((" [%d]\n", slot));
		return slot;
	    }
	}


	{
	    /* check for a user-defined encoding */
	    filename = pdf_find_resource(p, "Encoding", encoding, pdf_true);
	    if (filename == NULL) {
		if (p->debug['F'] == pdf_true) {
		    pdf_error(p, PDF_RuntimeError,
			    "Unknown encoding '%s'", encoding);
		} else {
		    PDF_TRACE((" [%d]\n", -1));
		    return -1;
		}
	    } else {
		enc = pdf_load_encoding(p, filename, encoding);
		if (enc == invalidenc) {
		    PDF_TRACE((" [%d]\n", -1));
		    return -1;
		}
	    }
	}
    }

    /* 
     * Look whether font is already in the cache.
     * If font with same encoding and embedding flag is found,
     * return its descriptor.
     */

    for (slot = 0; slot < p->fonts_number; slot++) {
	if (!strcmp(p->fonts[slot].name, fontname) &&
		    p->fonts[slot].encoding == enc) {
		PDF_TRACE((" [%d]\n", slot));
		return slot;
	}
    }
    
    /* Grow font cache if necessary.
     * If an error happens and the slot won't be used, we have
     * allocated too early but haven't done any harm.
     */

    if (slot >= p->fonts_capacity)
	pdf_grow_fonts(p);

    /* Multiple Master handling:
     * - strip MM parameters to build the master name
     * - the master name is used to find the metrics
     * - the instance name (client-supplied font name) is used in all places
     * - although the master name is used for finding the metrics, the
     *   instance name is stored in the font struct.
     */

    strcpy(mastername, fontname);

    /* A Multiple Master font was requested */
    if ((mmparam = strstr(mastername, "MM_")) != NULL) {
        if (embed)
            pdf_error(p, PDF_ValueError,
                "Font embedding for Multiple Master fonts not supported");
        mmparam[2] = '\0';      /* strip the parameter from the master name */
    }

    /* Try to locate font metrics in several places:
     * - in core (including CID fonts)
     * - dynamically configured AFM files
     * - dynamically configured PFM files
     * - Win32: TrueType host fonts
     * - dynamically configured TrueType files
     * - try to load resource file and try again to read AFMs, PFMs,
     *   and TrueTypes
     */

    /* try all available sources of font metrics */
    for ( /* */ ; /* */ ; /* */ )  {
	/* Check available in-core metrics */
	if (pdf_get_metrics_core(p, &p->fonts[slot], mastername, enc))
	    break;

	/* Check available AFM files (dynamically loaded) */
	filename = pdf_find_resource(p, "FontAFM", mastername, pdf_false);
	if (filename != NULL && pdf_get_metrics_afm(p, &p->fonts[slot],
		mastername, enc, filename))
	    break;

	/* Check available PFM files (dynamically loaded) */
	filename = pdf_find_resource(p, "FontPFM", mastername, pdf_false);
	if (filename != NULL && pdf_get_metrics_pfm(p, &p->fonts[slot],
		    mastername, enc, filename))
	    break;

#ifdef PDF_TRUETYPE_SUPPORTED

#if defined(PDF_HOSTFONTS_SUPPORTED)
	/* Check available TrueType fonts (installed on the host) */
	if (!p->debug['h'] &&
	    pdf_get_metrics_tt(p, &p->fonts[slot], mastername, enc, NULL,embed))
	    break;
#endif /* PDF_HOSTFONTS_SUPPORTED */

	/* Check available TrueType files (dynamically loaded) */
	filename = pdf_find_resource(p, "FontOutline", mastername, pdf_false);
	if (filename != NULL && pdf_get_metrics_tt(p, &p->fonts[slot],
		    mastername, enc, filename, embed))
	    break;
#endif /* PDF_TRUETYPE_SUPPORTED */

	/* Check available AFM files (including UPR file) */
	filename = pdf_find_resource(p, "FontAFM", mastername, pdf_true);
	if (filename != NULL && pdf_get_metrics_afm(p, &p->fonts[slot],
		mastername, enc, filename))
	    break;

	/* Check available PFM files (including UPR file) */
	filename = pdf_find_resource(p, "FontPFM", mastername, pdf_true);
	if (filename != NULL && pdf_get_metrics_pfm(p, &p->fonts[slot],
		    mastername, enc, filename))
	    break;

#ifdef PDF_TRUETYPE_SUPPORTED
	/* Check available TrueType files (including UPR file) */
	filename = pdf_find_resource(p, "FontOutline", mastername, pdf_true);
	if (filename != NULL && pdf_get_metrics_tt(p, &p->fonts[slot],
		    mastername, enc, filename, embed))
	    break;
#endif /* PDF_TRUETYPE_SUPPORTED */

	/* this conditional will exit the for loop in all cases */
	if (p->debug['F'] == pdf_true) {
	    pdf_error(p, PDF_RuntimeError,
		    "Metrics data for font '%s' not found", fontname);
	} else {
	    PDF_TRACE((" [%d]\n", -1));
	    return -1;
	}
    }

    /* store instance name instead of master name in the font structure */
    if (mmparam) {
        p->free(p, p->fonts[slot].name);
        p->fonts[slot].name = pdf_strdup(p, fontname);
    }

    /* If embedding was requested, find font file (or raise an exception)
     * - check host fonts
     * - Look for font file, using "FontOutline" entries from resource file
     */

    p->fonts[slot].fontfilename	= NULL;

    if (embed) {
	if (p->fonts[slot].type != Type1 &&
	    p->fonts[slot].type != MMType1 &&
	    p->fonts[slot].type != TrueType &&
	    p->fonts[slot].type != Type1C)
		pdf_error(p, PDF_SystemError, "Bad font type in PDF_findfont");

	/* search for file name if no host font available */
	if (!p->fonts[slot].img)
	    filename =
		pdf_find_resource(p, "FontOutline", mastername, pdf_true);

	/* font file not configured, and no host font available */
	if (filename == NULL && !p->fonts[slot].img) {
	    if (p->debug['F'] == pdf_true) {
		pdf_error(p, PDF_RuntimeError,
			"Outline data for font '%s' not found", fontname);
	    } else {
		PDF_TRACE((" [%d]\n", -1));
		return -1;
	    }
	} else
	    p->fonts[slot].fontfilename = filename;
    }

    /* Now everything is fine; fill the remaining font cache entries */

    p->fonts[slot].obj_id	= pdf_alloc_id(p);
    p->fonts[slot].embed	= embed;

    p->fonts_number++;

    /* return valid font descriptor */
    PDF_TRACE((" [%d]\n", slot));

    return slot;
}

#define LINEBUFLEN	256

static void
t1data_init(PDF *p, PDF_data_source *src)
{
    t1_private_data *t1_private;

    (void) p;

    t1_private = (t1_private_data *) src->private_data;

    t1_private->portion = ascii;
    t1_private->length1 = (size_t) 0;
    t1_private->length2 = (size_t) 0;
    t1_private->length3 = (size_t) 0;

    src->buffer_start	= NULL;
}

/*
 * PFA files are assumed to be encoded in host format. Therefore
 * we must use literal strings and characters for interpreting the
 * font file.
 */

static int
PFA_data_fill(PDF *p, PDF_data_source *src)
{
#ifndef PDFLIB_EBCDIC
    static const char HexToBin['F' - '0' + 1] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
	0, 10, 11, 12, 13, 14, 15
    }; 
#else
    static const char HexToBin['9' - 'A' + 1] = {
	10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0,  1,  2,  3,  4,  5, 6, 7, 8, 9
    };
#endif
    char *s, *c;
    int i;
    int len;
    t1_private_data *t1_private;

    t1_private = (t1_private_data *) src->private_data;

    if (src->buffer_start == NULL) {
	src->buffer_start = (pdf_byte *)p->malloc(p, LINEBUFLEN, "t1data_fill");
	src->buffer_length = LINEBUFLEN;
    }

    if ((s = fgets((char *) src->buffer_start, LINEBUFLEN, 
    	t1_private->fontfile)) == NULL)
	return pdf_false;

    len = (int) strlen(s);

    if (len >= 2 && s[len-1] == '\n' && s[len-2] == '\r') {
	s[len-2] = '\n';		/* adjust DOS-style line ends */
	len--;
    } else if (len >= 1 && s[len-1] == '\r') {
	s[len-1] = '\n';		/* adjust Mac-style line ends */
    }

    /* check for line of zeros: set zero flag if found */
    if (*s == '0') {
	for (c = s; *c == '0'; c++)
	    /* nothing */ ;

	if (*c == '\n')
	    t1_private->portion = zeros;
    }

    src->next_byte = src->buffer_start;

    switch (t1_private->portion) {
	case ascii:
	    t1_private->length1 += (size_t) len;
	    src->bytes_available = (size_t) len;
#ifdef PDFLIB_EBCDIC
	    pdf_make_ascii_len((char *) src->next_byte, src->bytes_available);
#endif
	    break;

	case encrypted:
	    src->bytes_available = 0;

	    /* Convert to upper case for safe binary conversion */
	    for (c = s; *c != '\n'; c++) {
		    *c = (char) toupper(*c);
	    }

	    /* convert ASCII to binary in-place */
	    for (i=0; s[i] != '\n'; i += 2) {
		if ((!isxdigit(s[i]) && !isspace(s[i])) ||
		    (!isxdigit(s[i+1]) && !isspace(s[i+1]))) {
		    fclose(t1_private->fontfile);
		    pdf_error(p, PDF_IOError, 
			"Bogus Type 1 font ASCII data (0x%02X, 0x%02X)",
			s[i], s[i+1]);
		}
#ifndef PDFLIB_EBCDIC
		s[i/2] = (char) (16*HexToBin[s[i]-'0'] + HexToBin[s[i+1]-'0']);
#else
		s[i/2] = (char) (16*HexToBin[s[i]-'A'] + HexToBin[s[i+1]-'A']);
#endif
		src->bytes_available++;
	    }
	    t1_private->length2 += src->bytes_available;
	    break;

	case zeros:
	    t1_private->length3 += (size_t) len;
	    src->bytes_available = (size_t) len;
#ifdef PDFLIB_EBCDIC
	    pdf_make_ascii_len((char *) src->next_byte, src->bytes_available);
#endif
	    break;
    }

#define PDF_CURRENTFILE	"currentfile eexec"

    if (t1_private->portion != encrypted &&
		!strncmp((const char *)s, PDF_CURRENTFILE, 17))
	t1_private->portion = encrypted;

    return pdf_true;
}

#define PFB_MARKER	0x80
#define PFB_ASCII	1
#define PFB_BINARY	2
#define PFB_EOF		3

static size_t
pdf_read_pfb_segment_length(PDF *p, FILE *pfb)
{
    size_t length;

    (void) p;

    length  = (size_t) (getc(pfb) & 0xff);
    length |= (size_t) (getc(pfb) & 0xff) << 8;
    length |= (size_t) (getc(pfb) & 0xff) << 16;
    length |= (size_t) (getc(pfb) & 0xff) << 24;

    return length;
}

static int
PFB_data_fill(PDF *p, PDF_data_source *src)
{
    t1_private_data *t1;
    unsigned char c, type;
    pdf_bool err = pdf_false;

    t1 = (t1_private_data *) src->private_data;

    c		= (unsigned char) getc(t1->fontfile);
    type	= (unsigned char) getc(t1->fontfile);

    if (t1->length1 == (size_t) 0) {
	if (c != PFB_MARKER || type != PFB_ASCII) {
	    err = pdf_true;
	} else {
	    t1->length1 = pdf_read_pfb_segment_length(p, t1->fontfile);
	    src->buffer_start = (pdf_byte *) p->malloc(p, t1->length1,
	    			"PFB_data_fill");
	    if (fread(src->buffer_start, 1, t1->length1, t1->fontfile)
	    		!= t1->length1)
		err = pdf_true;
	    src->next_byte = src->buffer_start;
	    src->bytes_available = t1->length1;
	}

    } else if (t1->length2 == (size_t) 0) {
	if (c != PFB_MARKER || type != PFB_BINARY) {
	    err = pdf_true;
	} else {
	    t1->length2 = pdf_read_pfb_segment_length(p, t1->fontfile);
	    p->free(p, (void *) src->buffer_start);
	    src->buffer_start = (pdf_byte *) p->malloc(p, t1->length2,
	    			"PFB_data_fill");
	    if (fread(src->buffer_start, 1, t1->length2, t1->fontfile)
	    		!= t1->length2)
		err = pdf_true;
	    src->next_byte = src->buffer_start;
	    src->bytes_available = t1->length2;
	}

    } else if (t1->length3 == 0) {
	if (c != PFB_MARKER || type != PFB_ASCII) {
	    err = pdf_true;
	} else {
	    t1->length3 = pdf_read_pfb_segment_length(p, t1->fontfile);
	    p->free(p, (void *) src->buffer_start);
	    src->buffer_start = (pdf_byte *) p->malloc(p, t1->length3,
	    				"PFB_data_fill");
	    if (fread(src->buffer_start, 1, (size_t) t1->length3, t1->fontfile)
	    		!= (size_t) t1->length3)
		err = pdf_true;
	    src->next_byte = src->buffer_start;
	    src->bytes_available = t1->length3;
	}
    } else if (c != PFB_MARKER || type != PFB_EOF) {
	err = pdf_true;
    } else {
	return pdf_false;
    }

    if (err) {
	fclose(t1->fontfile);
	pdf_error(p, PDF_IOError, "Corrupt PFB font file");
    }

    return pdf_true;
}

static void
t1data_terminate(PDF *p, PDF_data_source *src)
{
    p->free(p, (void *) src->buffer_start);
}

void
pdf_write_encoding(PDF *p, pdf_encodingvector *encoding, id encoding_id)
{
    int i;

    pdf_begin_obj(p, encoding_id);		/* Encoding object */
    pdf_begin_dict(p);

    pdf_puts(p, "/Type/Encoding\n");
    pdf_puts(p, "/Differences[0\n");

    for (i=0; i < 256; i++)
	pdf_printf(p, "/%s\n",
		(encoding->chars)[i] ? (encoding->chars)[i] : ".notdef");

    pdf_puts(p, "]\n");

    pdf_end_dict(p);
    pdf_end_obj(p);				/* Encoding object */

    if (p->flush & PDF_FLUSH_CONTENT)
	pdf_flush_stream(p);
}

void
pdf_make_fontflag(PDF *p, pdf_font *font)
{
    (void) p;

    if (font->type != Type1 && font->type != MMType1 &&
	font->type != TrueType && font->type != Type1C)
	return;

    if (font->isFixedPitch)
	font->flags |= FIXEDWIDTH;

    if (!strcmp(font->encodingScheme, "AdobeStandardEncoding"))
	font->flags |= ADOBESTANDARD;
    else
	font->flags |= SYMBOL;

    if (font->italicAngle < 0)
	font->flags |= ITALIC;

    /* heuristic to identify (small) caps fonts */
    if (font->name && 
	(strstr(font->name, "Caps") ||
	!strcmp(font->name + strlen(font->name) - 2, "SC")))
	font->flags |= SMALLCAPS;

    if (strstr(font->name, "Bold") || font->StdVW > PDF_STEMV_SEMIBOLD)
	font->flags |= FORCEBOLD;
}

void
pdf_write_fontdescriptor(
    PDF *p,
    pdf_font *font,
    id fontdescriptor_id,
    id fontfile_id)
{
#ifdef PDFLIB_EBCDIC
	char afontname[128];
#endif

    /* write the font descriptor with a preallocated id */

    pdf_begin_obj(p, fontdescriptor_id);	/* font descriptor */
    pdf_begin_dict(p);
    pdf_puts(p, "/Type/FontDescriptor\n");
    pdf_printf(p, "/Ascent %d\n", font->ascender);
    pdf_printf(p, "/CapHeight %d\n", font->capHeight);
    pdf_printf(p, "/Descent %d\n", font->descender);
    pdf_printf(p, "/Flags %ld\n", font->flags);
    pdf_printf(p, "/FontBBox[%d %d %d %d]\n",
	(int) font->llx, (int) font->lly, (int) font->urx, (int) font->ury);

    pdf_printf(p, "/FontName/");
#ifndef PDFLIB_EBCDIC
    pdf_quote_name(p, font->name, strlen(font->name));
#else
    strcpy(afontname, font->name);
    pdf_make_ascii(afontname);
    pdf_quote_name(p, afontname, strlen(afontname));
#endif
    pdf_puts(p, "\n");

    pdf_printf(p, "/ItalicAngle %d\n", (int) (font->italicAngle));
    pdf_printf(p, "/StemV %d\n", font->StdVW);

    if (font->StdHW > 0)
	pdf_printf(p, "/StemH %d\n", font->StdHW);

    if (font->xHeight > 0)
	pdf_printf(p, "/XHeight %d\n", font->xHeight);

    if (fontfile_id != BAD_ID) {
	if (font->type == Type1 || font->type == MMType1)
	    pdf_printf(p, "/FontFile %ld 0 R\n", fontfile_id);
	else if (font->type == TrueType)
	    pdf_printf(p, "/FontFile2 %ld 0 R\n", fontfile_id);
	else if (font->type == Type1C)
	    pdf_printf(p, "/FontFile3 %ld 0 R\n", fontfile_id);
	else
	    pdf_error(p, PDF_SystemError,
		"Bad font type in pdf_write_fontdescriptor");
    }

    pdf_end_dict(p);
    pdf_end_obj(p);				/* font descriptor */
}

static void
pdf_put_t1font(PDF *p, int slot)
{
    FILE	*fontfile = NULL;
    int		i, j;
    id		encoding_id = BAD_ID, fontdescriptor_id, fontfile_id;
    id		length_id, length1_id, length2_id, length3_id;
    long	length, stream_start;
    PDF_data_source t1src;
    const char	**cp;
    pdf_font	*font;
#ifdef PDFLIB_EBCDIC
    char	*afontname;
#endif

    font	= &p->fonts[slot];

    /* check whether we have one of the base 14 fonts */
    if (font->embed == 0) {
	for (cp = pdf_base14_names; *cp; ++cp) {
	    if (!strcmp(*cp, font->name)) {
		pdf_begin_obj(p, font->obj_id);	/* font */
		pdf_begin_dict(p);

		pdf_puts(p, "/Type/Font\n");
		pdf_puts(p, "/Subtype/Type1\n");

		if (font->encoding != builtin) {
		    if (p->encodings[font->encoding]->pdfname)
			pdf_printf(p, "/Encoding/%s\n", 
				    p->encodings[font->encoding]->pdfname);
		    else {
			encoding_id = pdf_alloc_id(p);
			pdf_printf(p, "/Encoding %ld 0 R\n", encoding_id);
		    }
		}

		pdf_printf(p, "/BaseFont/%s\n", *cp);

		pdf_end_dict(p);
		pdf_end_obj(p);			/* font */

		if (font->encoding != builtin &&
		    !p->encodings[font->encoding]->pdfname)
			pdf_write_encoding(p, p->encodings[font->encoding],
			    encoding_id);
		return;
	    }
	}
    }

    /* check for font file */
    if (font->embed == 1) {
	if ((fontfile = fopen(font->fontfilename, READMODE)) == NULL)
	    pdf_error(p, PDF_IOError, "Couldn't open font file '%s'",
	    			font->fontfilename);
    }

    pdf_begin_obj(p, font->obj_id);		/* font attributes */
    pdf_begin_dict(p);
    pdf_puts(p, "/Type/Font\n");

    if (font->type == Type1)
	pdf_puts(p, "/Subtype/Type1\n");
    else if (font->type == MMType1)
	pdf_puts(p, "/Subtype/MMType1\n");
    else
	pdf_error(p, PDF_SystemError, "Bad font type in pdf_put_t1font");

    pdf_puts(p, "/FirstChar 0\n");
    pdf_puts(p, "/LastChar 255\n");

    pdf_puts(p, "/Widths[\n");

    for (i = 0; i < 16; i++) {
	for (j = 0; j < 16; j++)
	    pdf_printf(p, " %d", font->widths[16*i + j]);
	pdf_putc(p, PDF_NEWLINE);
    }

    pdf_puts(p, "]\n");

    if (font->encoding != builtin) {
	if (p->encodings[font->encoding]->pdfname)
	    pdf_printf(p, "/Encoding/%s\n",
	    	p->encodings[font->encoding]->pdfname);
	else {
	    encoding_id = pdf_alloc_id(p);
	    pdf_printf(p, "/Encoding %ld 0 R\n", encoding_id);
	}
    }

    pdf_puts(p, "/BaseFont/");
#ifndef PDFLIB_EBCDIC
    pdf_quote_name(p, font->name, strlen(font->name));
#else
    afontname = pdf_strdup(p, font->name);
    pdf_make_ascii(afontname);
    pdf_quote_name(p, afontname, strlen(afontname));
#endif
    pdf_puts(p, "\n");

    fontdescriptor_id = pdf_alloc_id(p);
    pdf_printf(p, "/FontDescriptor %ld 0 R\n", fontdescriptor_id);

    pdf_end_dict(p);
    pdf_end_obj(p);			/* font attributes */

    if (font->embed == 1)
	fontfile_id = pdf_alloc_id(p);
    else
	fontfile_id = BAD_ID;

    pdf_write_fontdescriptor(p, font, fontdescriptor_id, fontfile_id);

    if (font->encoding != builtin && !p->encodings[font->encoding]->pdfname)
	pdf_write_encoding(p, p->encodings[font->encoding], encoding_id);

    /* font files */
    if (font->embed == 1) {
	unsigned char magic;

	pdf_begin_obj(p, fontfile_id);
	pdf_begin_dict(p);

	if (p->debug['a'])
	    pdf_puts(p, "/Filter/ASCIIHexDecode\n");

	length_id = pdf_alloc_id(p);
	length1_id = pdf_alloc_id(p);
	length2_id = pdf_alloc_id(p);
	length3_id = pdf_alloc_id(p);
	pdf_printf(p, "/Length %ld 0 R\n", length_id);
	pdf_printf(p, "/Length1 %ld 0 R\n", length1_id);
	pdf_printf(p, "/Length2 %ld 0 R\n", length2_id);
	pdf_printf(p, "/Length3 %ld 0 R\n", length3_id);

	/* Don't omit zeros since more data may follow at the end! */
	pdf_end_dict(p);

	pdf_begin_stream(p);			/* font data stream */
	stream_start = pdf_tell(p);

	magic = (unsigned char) getc(fontfile);
	(void) ungetc(magic, fontfile);

	t1src.init	= t1data_init;
	t1src.fill	= (magic == PFB_MARKER ? PFB_data_fill : PFA_data_fill);
	t1src.terminate	= t1data_terminate;

	t1src.private_data = (unsigned char *)
		p->malloc(p, sizeof(t1_private_data), "pdf_put_t1font");
	((t1_private_data *) t1src.private_data)->fontfile = fontfile;

	if (p->debug['a']) {
	    pdf_ASCIIHexEncode(p, &t1src);
	} else {
	    t1src.init(p, &t1src);

	    while (t1src.fill(p, &t1src))
		pdf_write(p, t1src.next_byte, t1src.bytes_available);

	    t1src.terminate(p, &t1src);
	}

	length = pdf_tell(p) - stream_start;
	pdf_end_stream(p);
	pdf_end_obj(p);				/* font file object */

	pdf_begin_obj(p, length_id);		/* Length object */
	pdf_printf(p, "%ld\n", length);
	pdf_end_obj(p);

	pdf_begin_obj(p, length1_id);		/* Length1 object */
	pdf_printf(p, "%ld\n", 
		((t1_private_data *) t1src.private_data)->length1);
	pdf_end_obj(p);

	pdf_begin_obj(p, length2_id);		/* Length2 object */
	pdf_printf(p, "%ld\n",
		((t1_private_data *) t1src.private_data)->length2);
	pdf_end_obj(p);

	pdf_begin_obj(p, length3_id);		/* Length3 object */
	pdf_printf(p, "%ld\n",
		((t1_private_data *) t1src.private_data)->length3);
	pdf_end_obj(p);

	fclose(((t1_private_data *) t1src.private_data)->fontfile);
	p->free(p, (void *) t1src.private_data);
    }

    if (p->flush & PDF_FLUSH_CONTENT)
	pdf_flush_stream(p);
}

static void
pdf_put_cidfont(PDF *p, int slot)
{
    id		descendant_id, fontdescriptor_id;
    pdf_font	*font;

    font	= &p->fonts[slot];

    pdf_begin_obj(p, font->obj_id);		/* font attributes */
    pdf_begin_dict(p);				/* Type 0 font dict */

    pdf_puts(p, "/Type/Font\n");
    pdf_puts(p, "/Subtype/Type0\n");
    pdf_printf(p, "/BaseFont/%s-%s\n", font->name, font->encodingScheme);
    pdf_printf(p, "/Encoding/%s\n", font->encodingScheme);

    descendant_id = pdf_alloc_id(p);
    pdf_printf(p, "/DescendantFonts[%ld 0 R]\n", descendant_id);

    pdf_end_dict(p);				/* Type 0 font dict */
    pdf_end_obj(p);				/* font attributes */

    pdf_begin_obj(p, descendant_id);		/* CIDFontType0 object */
    pdf_begin_dict(p);

    pdf_puts(p, "/Type/Font\n");
    pdf_puts(p, "/Subtype/CIDFontType0\n");
    pdf_printf(p, "/BaseFont/%s\n", font->name);

    fontdescriptor_id = pdf_alloc_id(p);
    pdf_printf(p, "/FontDescriptor %ld 0 R\n", fontdescriptor_id);

    if (font->charcoll >= cc_identity)
	pdf_error(p, PDF_SystemError, "Bad character collection");

    pdf_printf(p,
	"/CIDSystemInfo<</Registry(Adobe)/Ordering(%s)/Supplement %d>>\n",
	charcoll_names[font->charcoll].ordering,
	charcoll_names[font->charcoll].supplement);

    pdf_end_dict(p);
    pdf_end_obj(p);				/* CIDFontType0 object */

    pdf_write_fontdescriptor(p, font, fontdescriptor_id, BAD_ID);
}

void
pdf_write_doc_fonts(PDF *p)
{
    int	slot;

    /* output pending font objects */
    for (slot = 0; slot < p->fonts_number; slot++) {
	if (p->fonts[slot].type == Type1 || p->fonts[slot].type == MMType1)
	    pdf_put_t1font(p, slot);
#ifdef PDF_TRUETYPE_SUPPORTED
	else if (p->fonts[slot].type == TrueType ||
		p->fonts[slot].type == Type1C)
	    pdf_put_ttfont(p, slot);
#endif /* PDF_TRUETYPE_SUPPORTED */
	else if (p->fonts[slot].type == CIDFontType0)
	    pdf_put_cidfont(p, slot);
	else
	    pdf_error(p, PDF_SystemError,
		"Unsupported font type in pdf_write_doc_fonts");
    }
}

void
pdf_init_font_struct(PDF *p, pdf_font *font)
{
    (void) p;

    /*
     * Fill in some reasonable default values in global font info in 
     * case they're missing from the metrics file.
     */

    font->used_on_current_page	= pdf_false;
    font->type 			= Type1;

    font->flags			= 0L;
    font->ascender		= 800;
    font->descender		= -200;
    font->capHeight		= 700;
    font->xHeight		= 0;
    font->isFixedPitch		= pdf_false;
    font->italicAngle		= 0;
    font->llx			= (float) -50;
    font->lly			= (float) -200;
    font->urx			= (float) 1000;
    font->ury			= (float) 900;
    font->StdHW			= 0;
    font->StdVW			= 0;
    font->underlinePosition	= -100;
    font->underlineThickness	= 50;

    font->name			= NULL;
    font->ttname		= NULL;
    font->charcoll		= cc_none;
    font->encodingScheme 	= NULL;
    font->encoding	 	= builtin;

    font->numOfChars		= 0;
    font->cmi			= NULL;
    font->numOfTracks		= 0;
    font->tkd			= NULL;
    font->numOfPairs		= 0;
    font->pkd			= NULL;
    font->numOfComps		= 0;
    font->ccd			= NULL;

    font->filelen		= 0L;
    font->img			= (pdf_byte *) NULL;
}

PDFLIB_API const char * PDFLIB_CALL
PDF_encoding_get_name(PDF *p, const char *encoding, int slot)
{
    static const char fn[] = "PDF_encoding_get_name";
    int enc;

    PDF_TRACE(("%s\t(pdf[%p], \"%s\", %04X);\n",
	fn, (void *) p, encoding, slot));

    if (PDF_SANITY_CHECK_FAILED(p))
	return NULL;

    PDF_CHECK_SCOPE(p, fn, pdf_state_document | pdf_state_ppt | pdf_state_path);

    if (!strcmp(encoding, "builtin") || !strcmp(encoding, "host"))
	pdf_error(p, PDF_ValueError, "Can't query encoding '%s'", encoding);

    if (slot < 0 || slot > 255)
	pdf_error(p, PDF_ValueError,
	    "Illegal slot number %d in PDF_encoding_get_name", slot);

    /* search for a registered encoding */
    for (enc = 0; enc < ENC_MAX && p->encodings[enc] != NULL; enc++) {
	if (p->encodings[enc]->apiname &&
	    !strcmp(encoding, p->encodings[enc]->apiname))
	    break;
    }

    if (enc == ENC_MAX || p->encodings[enc] == NULL)
	pdf_error(p, PDF_ValueError,
	    "Encoding '%s' not found in PDF_encoding_get_name", encoding);
    
    if (p->encodings[enc]->chars[slot])
	return p->encodings[enc]->chars[slot];
    else
	return ".notdef";
}
