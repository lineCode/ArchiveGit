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

/* $Id: p_filter.c,v 1.10.2.3 2002/01/07 18:26:29 tm Exp $
 *
 * ASCII85 and Hex encoding for PDFlib 
 *
 */

#include <stdio.h>

#include "p_intern.h"

#ifdef PDF_ASCII85_USED

/* output one ASCII byte and keep track of characters per line */
static void 
pdf_outbyte(PDF *p, pdf_byte c)
{
    pdf_putc(p, (char) c);

#define MAX_CHARS_PER_LINE	64
    /* insert line feed */
    if (++(p->chars_on_this_line) == MAX_CHARS_PER_LINE)
    {
	pdf_putc(p, PDF_NEWLINE);
	p->chars_on_this_line = 0;
    }
}

static const unsigned long power85[5] = 
	{ 1L, 85L, 85L*85, 85L*85*85, 85L*85*85*85};

void
pdf_ASCII85Encode(PDF *p, PDF_data_source *src)
{
    unsigned long word, v;
    int i, fetched;
    pdf_byte buf[4];

    src->init(p, src);
    if (!src->fill(p, src)) {
	pdf_error(p, PDF_IOError, "Data underrun in pdf_ASCII85Encode");
	return;
    }

    p->chars_on_this_line = 0;

    for (;;) {
	for (fetched = 0; fetched < 4; fetched++)
	{
	    if (src->bytes_available == 0 && !src->fill(p, src))
		break;
	    buf[fetched] = *(src->next_byte);
	    src->next_byte++;
	    src->bytes_available--;
	}
	if (fetched < 4)
	    break;

	/* 4 bytes available ==> output 5 bytes */
	word = ((unsigned long)(((unsigned int)buf[0] << 8) + buf[1]) << 16) +
	       (((unsigned int)buf[2] << 8) + buf[3]);
	if (word == 0)
	    pdf_outbyte(p, (pdf_byte) PDF_z);       /* shortcut for 0 */
	else
	{
	    /* calculate 5 ASCII85 bytes and output them */
	    for (i = 4; i >= 0; i--) {
		v = word / power85[i];
		pdf_outbyte(p, (pdf_byte) (v + PDF_EXCLAM));
		word -= v * power85[i];
	    }
	}
    }

    word = 0;

    /* 0-3 bytes left */
    if (fetched != 0)
    {
	for (i = fetched - 1; i >= 0; i--)   /* accumulate bytes */
	    word += (unsigned long)buf[i] << 8 * (3-i);

	/* encoding as above, but output only fetched+1 bytes */
	for (i = 4; i >= 4-fetched; i--)
	{
	    v = word / power85[i];
	    pdf_outbyte(p, (pdf_byte)(v + PDF_EXCLAM));
	    word -= v * power85[i];
	}
    }

    src->terminate(p, src);

    pdf_puts(p, "~>\n");		/* EOD marker */
}

#endif /* PDF_ASCII85_USED */

void 
pdf_ASCIIHexEncode(PDF *p, PDF_data_source *src)
{
    static const char BinToHex[] = PDF_STRING_0123456789ABCDEF;
    int CharsPerLine;
    size_t i;
    pdf_byte *data;

    CharsPerLine = 0;

    src->init(p, src);

    while (src->fill(p, src))
    {
	for (data=src->next_byte, i=src->bytes_available; i > 0; i--, data++)
	{
	  pdf_putc(p, BinToHex[*data>>4]);     /* first nibble  */
	  pdf_putc(p, BinToHex[*data & 0x0F]); /* second nibble */
	  if ((CharsPerLine += 2) >= 64) {
	    pdf_putc(p, PDF_NEWLINE);
	    CharsPerLine = 0;
	  }
	}
    }

    src->terminate(p, src);
    pdf_puts(p, ">\n");         /* EOD marker for PDF hex strings */
}

/* methods for constructing a data source from a file */

#define FILE_BUFSIZE 1024

void
pdf_data_source_file_init(PDF *p, PDF_data_source *src)
{
    FILE		*fp;

    src->buffer_length = FILE_BUFSIZE;
    src->buffer_start = (pdf_byte *)
	p->malloc(p, src->buffer_length, "PDF_data_source_file_init");

    fp = fopen((char *) src->private_data, READMODE);

    if (fp == NULL)
	pdf_error(p, PDF_IOError,
	    "Couldn't open file '%s'", src->private_data);

    if (src->offset)
	fseek(fp, src->offset, SEEK_SET);

    src->private_data = (void *) fp;
    src->total = (long) 0;
}

pdf_bool
pdf_data_source_file_fill(PDF *p, PDF_data_source *src)
{
    size_t	bytes_needed;
    (void) p;	/* avoid compiler warning "unreferenced parameter" */

    if (src->length != (long) 0 && src->total + FILE_BUFSIZE > src->length)
	bytes_needed = (size_t) (src->length - src->total);
    else
	bytes_needed = FILE_BUFSIZE;

    src->next_byte = src->buffer_start;
    src->bytes_available = fread(src->buffer_start, 1,
    			bytes_needed, (FILE *) (src->private_data));

    src->total += (long) src->bytes_available;

    if (src->bytes_available == 0)
	return pdf_false;
    else
	return pdf_true;
}

void
pdf_data_source_file_terminate(PDF *p, PDF_data_source *src)
{
    p->free(p, (void *) src->buffer_start);
    fclose((FILE *) src->private_data);

    if (src->length != (long) 0 && src->total != src->length)
	pdf_error(p, PDF_IOError,
	    "Couldn't read all required bytes from file '%s'",
	    src->private_data);
}

/* methods for constructing a data source from a memory buffer */

/* dummy for use in PDF_data_source */
void
pdf_noop(PDF *p, PDF_data_source *src)
{
    (void) p;	/* avoid compiler warning "unreferenced parameter" */
    (void) src;	/* avoid compiler warning "unreferenced parameter" */
}

int
pdf_data_source_buf_fill(PDF *p, PDF_data_source *src)
{
    (void) p;	/* avoid compiler warning "unreferenced parameter" */

    if (src->next_byte == NULL) {
	src->next_byte		= src->buffer_start;
	src->bytes_available	= src->buffer_length;
	return pdf_true;
    }

    return pdf_false;
}

/* copy the complete contents of src to the output */
void
pdf_copy(PDF *p, PDF_data_source *src)
{
    src->init(p, src);

    while (src->fill(p, src))
	pdf_write(p, src->next_byte, src->bytes_available);
    
    src->terminate(p, src);
}

void
pdf_compress(PDF *p, PDF_data_source *src)
{
    pdf_compress_init(p);

    pdf_copy(p, src);

    pdf_compress_end(p);
}
