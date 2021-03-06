// id's for Image Browser  All id's are from 5000-7000



// TYPES AND MISC 5000-5099	 
// any changes to file types should be reflected in RDRDEF in handlebsr.c

#define IDC_BSR_FILETYPE    5000
#define IDT_FIRSTFILETYPE	5001
#define IDT_ALLFILES	    5001
#define IDT_ALLIMAGES	    5002
#define IDT_FIRSTREAL	    5003	// first actual file type
#define IDT_PPCOMPOUND      5003
#define IDT_PP			    5004
#define IDT_TIFF		    5005
#define IDT_TARGA		    5006
#define IDT_GIF			    5007
#define IDT_BMP			    5008
#define IDT_DIB			    5009
#define IDT_PCX			    5010
#define IDT_EPS			    5011
#define IDT_DCS			    5012
#define IDT_JPEG		    5013
#define IDT_PHOTOCD		    5014
#define IDT_SCITEX          5015
#define IDT_AVI			    5016
#define IDT_PP4			    5017
#define IDT_LASTREAL	    IDT_PP4
#define IDT_LASTFILETYPE    IDT_PP4
#define IDT_NOFILETYPE	    (IDT_LASTFILETYPE+1)

// DATATYPES 
#define IDC_BSR_DATATYPE	5019
#define IDC_FIRSTDATATYPE	5020
#define IDC_SAVECT			5020
#define IDC_SAVELA			5021
#define IDC_SAVESP			5022
#define IDC_SAVE4BITCOLOR	5023
#define IDC_SAVE8BITCOLOR	5024
#define IDC_SAVE24BITCOLOR	5025
#define IDC_SAVE32BITCOLOR	5026
#define IDC_LASTDATATYPE	5026

#define IDT_FIRSTIMPORTFILETYPE IDT_ALLIMPORTFILES
#define IDT_ALLIMPORTFILES	    5048
#define IDT_ALLIMPORTIMAGES	    5049
#define IDT_FIRSTIMPORTREAL	    IDT_AI	// first actual file type
#define IDT_AI                  5050
#define IDT_CDR                 5051
#define IDT_CGM                 5052
#define IDT_DRW                 5053
#define IDT_WMF                 5054
#define IDT_LASTIMPORTREAL	    IDT_WMF
#define IDT_LASTIMPORTFILETYPE  IDT_WMF
#define IDT_NOIMPORTFILETYPE	(IDT_LASTIMPORTFILETYPE+1)

// menu items 5100-5299
#define IDM_BSR_ALB_MENU		3 		/*is the 4th popup*/

// menu popup id's (for hints) must be before thier first item
#define IDM_BSR_FILEMENU		5100
#define IDM_BSR_COPY			5101
#define IDM_BSR_DIRMENU			5102
#define IDM_BSR_CREATEDIR		5103
#define IDM_BSR_EDITMENU		5104
#define IDM_BSR_SELECTALL		5105
#define IDM_BSR_THMMENU			5106
#define IDM_BSR_THMUPDATE		5107
#define IDM_BSR_ALBMENU 		5108
#define IDM_BSR_ALBNEW			5109

#define IDM_BSR_MOVE			5110
#define IDM_BSR_RENAME			5111
#define IDM_BSR_DELETE			5112
#define IDM_BSR_ADDTOALB		5113
#define IDM_BSR_CONVERT			5114
#define IDM_BSR_INFO			5115
#define IDM_BSR_RENAMEDIR		5116
#define IDM_BSR_DELETEDIR		5117
#define IDM_BSR_UNDO			5118
#define IDM_BSR_SELECTNONE		5119
#define IDM_BSR_INVERTSELECT	5120
#define IDM_BSR_PREFERENCES		5121
#define IDM_BSR_ALBRENAME		5122
#define IDM_BSR_ALBDELETE		5123
#define IDM_BSR_ALBINFO			5124
#define IDM_BSR_ALBVIEW			5125
#define IDM_BSR_VIEWTHUMBS		5126
#define IDM_BSR_ALBIMPORT		5127
#define IDM_BSR_CLEANUP			5128
#define IDM_BSR_THMCREATE		5129
#define IDM_BSR_FIND			5130
#define IDM_BSR_SEARCH			5131
#define IDM_BSR_ALBEXPORT		5132
#define IDM_BSR_DIREXPORT		5133
#define IDM_BSR_DIRIMPORT		5134
#define IDM_BSR_THMMOVE			5135
#define IDM_BSR_PRINTTHUMBS		5136
#define IDM_BSR_REMOVEFROMALB	5137
#define IDM_BSR_THMDEL			5138
#define MENU_1 					5140
#define MENU_2 					5141
							   
// browser controls 5300-5499	   

#define IDC_BSR_PATH_TEXT 			5300
#define IDC_BSR_FILENAME			5301
#define IDC_BSR_FILELIST			5302
#define IDC_VIEW					5303
#define IDC_BSR_DRIVE				5304
#define IDC_BSR_DIRECTORIES			5305
#define IDC_BSR_FILESIZE			5306
#define IDC_BSR_FILEDATE			5307
#define IDC_BSR_FILETIME			5308
#define IDC_OK						5309
#define IDC_BSR_COMPRESS			5310
#define IDC_BSR_DESCRIPTION			5311
#define IDC_BSR_DRIVESIZE			5312
#define IDC_BSR_ALBUMCOMBO			5313
#define IDC_BSR_DIRPANE1			5314
#define IDC_BSR_DIRPANE2			5315
#define IDC_BSR_TEMPPANE1			5316
#define IDC_BSR_TEMPPANE2			5317
#define IDC_BSR_ALBUMS				5318
#define IDC_BSR_THUMBNAIL			5319
#define IDC_BSR_TEMPLIST			5320
#define IDC_BSR_THUMBS				5322
#define IDC_BSR_THM_SIZER			5323
#define IDC_BSR_NETWORK				5324


#define IDC_PATHICON_ROOT			5329
#define IDC_PATHICON_OPEN			5330
#define IDC_PATHICON_ACTIVE		    5331
#define IDC_PATHICON_CLOSED		    5332
#define IDC_PATHICON_FIXED			5333
#define IDC_PATHICON_NET			5334
#define IDC_PATHICON_FLOPPY		    5335
#define IDC_PATHICON_CDDRIVE		5336
#define IDC_PATHICON_RAMDRIVE		5337

// info dialog IDs
#define IDC_INFO_RESOLUTION			5358
#define IDC_INFO_COLORMANAGED		5359
#define IDC_INFO_FILENAME			5360
#define IDC_INFO_FILETIME			5361
#define IDC_INFO_FILESIZE			5362
#define IDC_INFO_FILEDATE			5363
#define IDC_INFO_THUMBNAIL			5365
#define IDC_INFO_IMAGEWIDTH			5366
#define IDC_INFO_IMAGEHEIGHT		5367
#define IDC_INFO_DATATYPE			5368
#define IDC_INFO_FILEDESCR			5369
#define IDC_INFO_SAVEDESCR			5370
#define IDC_INFO_PREVIOUS			5371
#define IDC_INFO_NEXT				5372
#define IDC_INFO_UPDATE				5373
#define IDC_INFO_FILETYPE			5374

#define IDC_ALBINFO_DESCR 			5375
#define IDC_ALBINFO_SAVEDESCR		5376
#define IDC_ALBINFO_NAME			5377
#define IDC_ALBINFO_NUMENTRIES		5378
#define IDC_ALBINFO_PREVIOUS		5379
#define IDC_ALBINFO_NEXT			5380

// for extended browser
#define IDC_INFO_SMALLFILENAME		5381
#define IDC_INFO_EXTNAME			5382


// other controls 5500-5599
#define	IDC_BSR_THMDIR				5500
#define	IDC_BSR_DBFINFODIR			5501
#define	IDC_BSR_ALBFILEDIR			5502
#define	IDC_BSR_TIFFCOMPRESS		5503
#define IDC_BSR_CONF_COPY			5505
#define IDC_BSR_CONF_MOVE			5506
#define IDC_BSR_CONF_RENAME			5507
#define IDC_BSR_CONF_DELETE			5508
#define IDC_BSR_CREATE_ON_SAVE		5509

#define	IDC_BSR_SAVEPREF			5514
#define	IDC_BSR_SAVEPATH			5515
#define IDC_BSR_AUTOTHM				5516
#define	IDC_ICON_ALBUMOPEN	  		5517
#define	IDC_ICON_ALBUMCLOSED 		5518

#define IDC_SEARCH_STRING			5519
#define IDC_SEARCH_BYFIRST			5525
#define IDC_SEARCH_BYKEY			5525
#define IDC_SEARCH_BYNAME			5526
#define IDC_SEARCH_BYLAST			5526
#define IDC_SEARCH_CASE				5527

#define IDC_SEARCH_SELECTEDALB		5533
#define IDC_SEARCH_ALLALB			5534
#define IDC_SEARCH_WHOLEDRIVE		5535
#define IDC_SEARCH_FROM_DIRECTORY 	5536

#define IDC_SEARCH_DIRECTORY	 	5537
#define IDC_SEARCH_NUMFOUND		 	5538
#define IDC_SEARCH_NUMSEARCHED		5539
#define IDC_SEARCH_NUMSEARCHED_TEXT	5540
#define IDC_SEARCH_SEARCH			5541
#define IDC_SEARCH_STOP				5542
#define IDC_SEARCH_SEARCHING		5543
#define IDC_CHOOSEALBUM_TEXT		5544

#define IDC_BSR_OPENMODE			5560
#define IDC_BSR_OPENNORMAL			5561
#define IDC_BSR_OPENFASTBITS		5562
#define IDC_BSR_OPENLOWRES			5563
#define IDC_BSR_FIRSTOPENMODE		IDC_BSR_OPENNORMAL
#define IDC_BSR_LASTOPENMODE		IDC_BSR_OPENLOWRES

#define IDC_BSR_PORTDIR				5564
#define IDC_BSR_PORTSUBDIR			5565
#define IDC_BSR_PORTCOPYTHMS		5566

#define IDC_BSR_THMLOCAL			5568
#define IDC_BSR_THMREMOTE			5569
#define IDC_BSR_THMOTHER			5570

#define IDC_BSR_LOCALDIR			5573
#define IDC_BSR_REMOTEDIR			5574
#define IDC_BSR_DELORIGINAL			5575

#define IDC_BSR_TEXT				5576
#define IDC_BSR_ALT_TEXT			5577
#define IDC_BSR_NO					5578
#define IDC_YESTOALL				5579
									
#define IDC_FILESRC					5581
#define IDC_FILEDST					5582

#define IDC_CLEAN_DIR				5583
#define IDC_CLEAN_ALB				5584
#define IDC_CLEAN_ALL				5585

#define IDC_CLEAN_SUBDIR			5586
#define IDC_CLEAN_DIR_TEXT			5587
#define IDC_CLEAN_ALB_TEXT			5588
#define IDC_BSR_PORTCOPYFILES		5589
#define IDC_BSR_PORTALB				5590
#define IDC_BSR_PORTFILEDIR			5591
#define IDC_BSR_PORTALBDIR			5592
#define IDC_DST_TEXT				5593

#define IDC_PT_USEROWS				5594
#define IDC_PT_USECOLUMNS			5595
#define IDC_PT_ROWS					5596
#define IDC_PT_COLUMNS				5597
#define IDC_PT_SELECTED				5598
#define IDC_ALBMANAGE				5599


									
// STRINGS 5700-5899 
#define IDS_TRUNCATE				5700
#define IDS_BSR_BLANK				5701
#define IDS_NEW_ALB					5702
#define IDS_NOALB					5703
#define IDS_ALB_EXISTS				5704
#define IDS_BADNAME					5705
#define	IDS_SAVE_TOOMANY			5706
#define IDS_BADALBNAME				5707
#define IDS_NAME_AMBIGUOUS			5708
#define IDS_DELETEALB				5709
#define IDS_RENAMEALB				5710
#define IDS_RENAMEALB_CONF			5711
#define IDS_VIEWDIR					5712
#define IDS_VIEWALB					5713
#define IDS_VIEWTHUMBS				5714
#define IDS_VIEWFILES				5715
#define IDS_BSR_ERROR				5716
#define IDS_EDELETETHUMBNAIL		5717
#define IDS_ERENAMETHUMBNAIL		5718
#define IDS_ECOPYTHUMBNAIL			5719
#define IDS_CONFIRMRENAME			5720
#define IDS_GETRENAMENAME			5721
#define IDS_GETCOPYNAME				5722
#define IDS_GETCOPYDIR				5723
#define IDS_EINVALIDDIR				5724
#define IDS_CONFIRMCOPY				5725
#define IDS_EACCES					5726
#define IDS_SAVEDESCR				5727
#define IDS_GETMOVENAME				5728
#define IDS_GETMOVEDIR				5729
#define IDS_CONFIRMMOVE				5730
#define IDS_EMOVETHUMBNAIL			5731
#define IDS_EMOVE					5732

#define IDS_GETNEWDIR				5733
#define IDS_CONFIRMNEWDIR			5734
#define IDS_ENEWDIR					5735
#define IDS_GETRENAMEDIR			5736
#define IDS_CONFIRMRENAMEDIR		5737
#define IDS_ERENAMEDIR				5738
#define IDS_CONFIRMDELETEDIR		5739
#define IDS_EDELETEDIR				5740
#define IDS_ECHANGETHUMBS			5741

#define IDS_SEARCH_EWRITEDBF		5742
#define IDS_SEARCH_ERROR			5743
#define IDS_NOTHUMB					5744
#define IDS_ADDTOALB_TITLE			5745
#define IDS_ADDTOALB_TEXT			5746
#define IDS_SEARCH_BUTTON			5747
#define IDS_STOP_BUTTON				5748
#define IDS_NO_DESCRIPTION			5749
#define IDS_BSR_INFOSIZE			5750

#define IDS_DIREXPORT_DUPLICATE		5751
#define IDS_DIRIMPORT_DUPLICATE		5752
#define IDS_ALBUM					5753
#define IDS_DIRECTORY				5754
#define IDS_BADDIR					5755
#define IDS_BADFILE					5756
#define IDS_SEARCH_DIRS				5757
#define IDS_SEARCH_FILES			5758	
#define IDS_SEARCH_DONE				5759
#define IDS_SEARCH_FAILED			5760	
#define IDS_BSR_INFORES			    5761

#define IDS_ALLFILES				5780
#define IDS_ALLIMAGES				5781
#define IDS_READONLY				5782
#define IDS_DIREXPORT_DUPTHUMBS		5783
#define IDS_ECOPY_SELF				5784
#define IDS_ERENAME_SELF			5785
#define IDS_CONF_DELETETHM			5786
#define IDS_EMOVE_SELF				5787
#define IDS_OVERWRITE				5788
#define IDS_ECANTDELETE				5789
#define IDS_INVALIDNAME				5790
#define IDS_NO_ALBUMS_HERE			5791
#define IDS_IMPORT_FILEOVERWRITE	5792
#define IDS_ALBUM_ALREADY_HERE		5793
#define IDS_NODELETE				5794
#define IDS_ECOPYCROSSDRIVE			5795
#define IDS_ALLIMPORTIMAGES			5796
#define IDS_ALLIMPORTFILES			5797

// Misc 5900-5999
#define IDD_BSR_PREFS				5900
#define IDD_BSR_FILEINFO			5901
#define IDD_BSR_ALBINFO				5902
#define IDD_BSR_SEARCH				5903
#define ID_BSR_ACCEL				5904
#define IDD_CHOOSEALBUM				5906
#define IDD_DIREXPORT				5907
#define IDD_DIRIMPORT				5908
#define IDD_THMMOVE					5909
#define IDD_CONF_FILEDEL			5910
#define IDD_CONF_DIRDEL				5911
#define IDD_MINI_OPEN 				5912
#define IDD_COPY_FILE				5913
#define IDD_COPY_FILES				5914
#define IDD_MOVE_FILE				5915
#define IDD_MOVE_FILES				5916
#define IDD_RENAME_FILE				5917
#define IDD_DELETE_VIRTUAL			5918
#define IDD_CLEANUP_THUMBS			5919
#define IDD_ALBEXPORT				5920
#define IDD_ALBIMPORT				5921
#define IDD_PRINT_THUMBS			5922
#define IDD_CONF_COPY				5923
#define IDD_CONF_MOVE				5924
#define IDD_GETRENAMEDIR			5925
#define IDD_GETNEWDIR				5926
#define IDD_RENAMEALB				5927
#define IDD_NEW_ALB					5928
#define IDD_DELETE_FILE				5929
#define IDD_DELETE_FILES			5930
#define IDD_CONF_REPLACE			5931
#define IDD_CONFIRM_IMPORTALB		5932	  
#define IDD_IMPORT_GETNEWNAME		5933
#define IDD_INTO_THIS_ALB			5934
#define IDD_INTO_ALB				5935
#define IDD_SAVENEWDIR				5936
#define IDD_ECOPYCROSSDRIVE			5937
