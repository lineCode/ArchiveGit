#include "stdafx.h"
#define GDIP_NO_EXTERN
#include "Gdip.h"

#define Using(name) name =(PFN##name)::GetProcAddress(CGdip::m_hGdiPlusLib, #name); ATLASSERT(name)
#define DeclareUsing(name) PFN##name name =(PFN##name)::GetProcAddress(CGdip::m_hGdiPlusLib, #name); ATLASSERT(name)

HMODULE CGdip::m_hGdiPlusLib = NULL;
bool CGdip::m_bInitFailed = false;
ULONG_PTR CGdip::m_dwToken = 0;
int CGdip::m_iRefCount = 0;

//////////////////////////////////////////////////////////////////////
bool CGdip::UsingList()
{
	Using(GdipAlloc);
	Using(GdipFree);

	Using(GdipCreatePath);
	Using(GdipCreatePath2);
	Using(GdipCreatePath2I);
	Using(GdipClonePath);
	Using(GdipDeletePath);
	Using(GdipResetPath);
	Using(GdipGetPointCount);
	Using(GdipGetPathTypes);
	Using(GdipGetPathPoints);
	Using(GdipGetPathPointsI);
	Using(GdipGetPathFillMode);
	Using(GdipSetPathFillMode);
	Using(GdipGetPathData);
	Using(GdipStartPathFigure);
	Using(GdipClosePathFigure);
	Using(GdipClosePathFigures);
	Using(GdipSetPathMarker);
	Using(GdipClearPathMarkers);
	Using(GdipReversePath);
	Using(GdipGetPathLastPoint);
	Using(GdipAddPathLine);
	Using(GdipAddPathLine2);
	Using(GdipAddPathArc);
	Using(GdipAddPathBezier);
	Using(GdipAddPathBeziers);
	Using(GdipAddPathCurve);
	Using(GdipAddPathCurve2);
	Using(GdipAddPathCurve3);
	Using(GdipAddPathClosedCurve);
	Using(GdipAddPathClosedCurve2);
	Using(GdipAddPathRectangle);
	Using(GdipAddPathRectangles);
	Using(GdipAddPathEllipse);
	Using(GdipAddPathPie);
	Using(GdipAddPathPolygon);
	Using(GdipAddPathPath);
	Using(GdipAddPathString);
	Using(GdipAddPathStringI);
	Using(GdipAddPathLineI);
	Using(GdipAddPathLine2I);
	Using(GdipAddPathArcI);
	Using(GdipAddPathBezierI);
	Using(GdipAddPathBeziersI);
	Using(GdipAddPathCurveI);
	Using(GdipAddPathCurve2I);
	Using(GdipAddPathCurve3I);
	Using(GdipAddPathClosedCurveI);
	Using(GdipAddPathClosedCurve2I);
	Using(GdipAddPathRectangleI);
	Using(GdipAddPathRectanglesI);
	Using(GdipAddPathEllipseI);
	Using(GdipAddPathPieI);
	Using(GdipAddPathPolygonI);
	Using(GdipFlattenPath);
	Using(GdipWindingModeOutline);
	Using(GdipWidenPath);
	Using(GdipWarpPath);
	Using(GdipTransformPath);
	Using(GdipGetPathWorldBounds);
	Using(GdipGetPathWorldBoundsI);
	Using(GdipIsVisiblePathPoint);
	Using(GdipIsVisiblePathPointI);
	Using(GdipIsOutlineVisiblePathPoint);
	Using(GdipIsOutlineVisiblePathPointI);
	Using(GdipCreatePathIter);
	Using(GdipDeletePathIter);
	Using(GdipPathIterNextSubpath);
	Using(GdipPathIterNextSubpathPath);
	Using(GdipPathIterNextPathType);
	Using(GdipPathIterNextMarker);
	Using(GdipPathIterNextMarkerPath);
	Using(GdipPathIterGetCount);
	Using(GdipPathIterGetSubpathCount);
	Using(GdipPathIterIsValid);
	Using(GdipPathIterHasCurve);
	Using(GdipPathIterRewind);
	Using(GdipPathIterEnumerate);
	Using(GdipPathIterCopyData);
	Using(GdipCreateMatrix);
	Using(GdipCreateMatrix2);
	Using(GdipCreateMatrix3);
	Using(GdipCreateMatrix3I);
	Using(GdipCloneMatrix);
	Using(GdipDeleteMatrix);
	Using(GdipSetMatrixElements);
	Using(GdipMultiplyMatrix);
	Using(GdipTranslateMatrix);
	Using(GdipScaleMatrix);
	Using(GdipRotateMatrix);
	Using(GdipShearMatrix);
	Using(GdipInvertMatrix);
	Using(GdipTransformMatrixPoints);
	Using(GdipTransformMatrixPointsI);
	Using(GdipVectorTransformMatrixPoints);
	Using(GdipVectorTransformMatrixPointsI);
	Using(GdipGetMatrixElements);
	Using(GdipIsMatrixInvertible);
	Using(GdipIsMatrixIdentity);
	Using(GdipIsMatrixEqual);
	Using(GdipCreateRegion);
	Using(GdipCreateRegionRect);
	Using(GdipCreateRegionRectI);
	Using(GdipCreateRegionPath);
	Using(GdipCreateRegionRgnData);
	Using(GdipCreateRegionHrgn);
	Using(GdipCloneRegion);
	Using(GdipDeleteRegion);
	Using(GdipSetInfinite);
	Using(GdipSetEmpty);
	Using(GdipCombineRegionRect);
	Using(GdipCombineRegionRectI);
	Using(GdipCombineRegionPath);
	Using(GdipCombineRegionRegion);
	Using(GdipTranslateRegion);
	Using(GdipTranslateRegionI);
	Using(GdipTransformRegion);
	Using(GdipGetRegionBounds);
	Using(GdipGetRegionBoundsI);
	Using(GdipGetRegionHRgn);
	Using(GdipIsEmptyRegion);
	Using(GdipIsInfiniteRegion);
	Using(GdipIsEqualRegion);
	Using(GdipGetRegionDataSize);
	Using(GdipGetRegionData);
	Using(GdipIsVisibleRegionPoint);
	Using(GdipIsVisibleRegionPointI);
	Using(GdipIsVisibleRegionRect);
	Using(GdipIsVisibleRegionRectI);
	Using(GdipGetRegionScansCount);
	Using(GdipGetRegionScans);
	Using(GdipGetRegionScansI);
	Using(GdipCloneBrush);
	Using(GdipDeleteBrush);
	Using(GdipGetBrushType);
	Using(GdipCreateHatchBrush);
	Using(GdipGetHatchStyle);
	Using(GdipGetHatchForegroundColor);
	Using(GdipGetHatchBackgroundColor);
	Using(GdipCreateTexture);
	Using(GdipCreateTexture2);
	Using(GdipCreateTextureIA);
	Using(GdipCreateTexture2I);
	Using(GdipCreateTextureIAI);
	Using(GdipGetTextureTransform);
	Using(GdipSetTextureTransform);
	Using(GdipResetTextureTransform);
	Using(GdipMultiplyTextureTransform);
	Using(GdipTranslateTextureTransform);
	Using(GdipScaleTextureTransform);
	Using(GdipRotateTextureTransform);
	Using(GdipSetTextureWrapMode);
	Using(GdipGetTextureWrapMode);
	Using(GdipGetTextureImage);
	Using(GdipCreateSolidFill);
	Using(GdipSetSolidFillColor);
	Using(GdipGetSolidFillColor);
	Using(GdipCreateLineBrush);
	Using(GdipCreateLineBrushI);
	Using(GdipCreateLineBrushFromRect);
	Using(GdipCreateLineBrushFromRectI);
	Using(GdipCreateLineBrushFromRectWithAngle);
	Using(GdipCreateLineBrushFromRectWithAngleI);
	Using(GdipSetLineColors);
	Using(GdipGetLineColors);
	Using(GdipGetLineRect);
	Using(GdipGetLineRectI);
	Using(GdipSetLineGammaCorrection);
	Using(GdipGetLineGammaCorrection);
	Using(GdipGetLineBlendCount);
	Using(GdipGetLineBlend);
	Using(GdipSetLineBlend);
	Using(GdipGetLinePresetBlendCount);
	Using(GdipGetLinePresetBlend);
	Using(GdipSetLinePresetBlend);
	Using(GdipSetLineSigmaBlend);
	Using(GdipSetLineLinearBlend);
	Using(GdipSetLineWrapMode);
	Using(GdipGetLineWrapMode);
	Using(GdipGetLineTransform);
	Using(GdipSetLineTransform);
	Using(GdipResetLineTransform);
	Using(GdipMultiplyLineTransform);
	Using(GdipTranslateLineTransform);
	Using(GdipScaleLineTransform);
	Using(GdipRotateLineTransform);
	Using(GdipCreatePathGradient);
	Using(GdipCreatePathGradientI);
	Using(GdipCreatePathGradientFromPath);
	Using(GdipGetPathGradientCenterColor);
	Using(GdipSetPathGradientCenterColor);
	Using(GdipGetPathGradientSurroundColorsWithCount);
	Using(GdipSetPathGradientSurroundColorsWithCount);
	Using(GdipGetPathGradientPath);
	Using(GdipSetPathGradientPath);
	Using(GdipGetPathGradientCenterPoint);
	Using(GdipGetPathGradientCenterPointI);
	Using(GdipSetPathGradientCenterPoint);
	Using(GdipSetPathGradientCenterPointI);
	Using(GdipGetPathGradientRect);
	Using(GdipGetPathGradientRectI);
	Using(GdipGetPathGradientPointCount);
	Using(GdipGetPathGradientSurroundColorCount);
	Using(GdipSetPathGradientGammaCorrection);
	Using(GdipGetPathGradientGammaCorrection);
	Using(GdipGetPathGradientBlendCount);
	Using(GdipGetPathGradientBlend);
	Using(GdipSetPathGradientBlend);
	Using(GdipGetPathGradientPresetBlendCount);
	Using(GdipGetPathGradientPresetBlend);
	Using(GdipSetPathGradientPresetBlend);
	Using(GdipSetPathGradientSigmaBlend);
	Using(GdipSetPathGradientLinearBlend);
	Using(GdipGetPathGradientWrapMode);
	Using(GdipSetPathGradientWrapMode);
	Using(GdipGetPathGradientTransform);
	Using(GdipSetPathGradientTransform);
	Using(GdipResetPathGradientTransform);
	Using(GdipMultiplyPathGradientTransform);
	Using(GdipTranslatePathGradientTransform);
	Using(GdipScalePathGradientTransform);
	Using(GdipRotatePathGradientTransform);
	Using(GdipGetPathGradientFocusScales);
	Using(GdipSetPathGradientFocusScales);
	Using(GdipCreatePen1);
	Using(GdipCreatePen2);
	Using(GdipClonePen);
	Using(GdipDeletePen);
	Using(GdipSetPenWidth);
	Using(GdipGetPenWidth);
	Using(GdipSetPenUnit);
	Using(GdipGetPenUnit);
	Using(GdipSetPenLineCap197819);
	Using(GdipSetPenStartCap);
	Using(GdipSetPenEndCap);
	Using(GdipSetPenDashCap197819);
	Using(GdipGetPenStartCap);
	Using(GdipGetPenEndCap);
	Using(GdipGetPenDashCap197819);
	Using(GdipSetPenLineJoin);
	Using(GdipGetPenLineJoin);
	Using(GdipSetPenCustomStartCap);
	Using(GdipGetPenCustomStartCap);
	Using(GdipSetPenCustomEndCap);
	Using(GdipGetPenCustomEndCap);
	Using(GdipSetPenMiterLimit);
	Using(GdipGetPenMiterLimit);
	Using(GdipSetPenMode);
	Using(GdipGetPenMode);
	Using(GdipSetPenTransform);
	Using(GdipGetPenTransform);
	Using(GdipResetPenTransform);
	Using(GdipMultiplyPenTransform);
	Using(GdipTranslatePenTransform);
	Using(GdipScalePenTransform);
	Using(GdipRotatePenTransform);
	Using(GdipSetPenColor);
	Using(GdipGetPenColor);
	Using(GdipSetPenBrushFill);
	Using(GdipGetPenBrushFill);
	Using(GdipGetPenFillType);
	Using(GdipGetPenDashStyle);
	Using(GdipSetPenDashStyle);
	Using(GdipGetPenDashOffset);
	Using(GdipSetPenDashOffset);
	Using(GdipGetPenDashCount);
	Using(GdipSetPenDashArray);
	Using(GdipGetPenDashArray);
	Using(GdipGetPenCompoundCount);
	Using(GdipSetPenCompoundArray);
	Using(GdipGetPenCompoundArray);
	Using(GdipCreateCustomLineCap);
	Using(GdipDeleteCustomLineCap);
	Using(GdipCloneCustomLineCap);
	Using(GdipGetCustomLineCapType);
	Using(GdipSetCustomLineCapStrokeCaps);
	Using(GdipGetCustomLineCapStrokeCaps);
	Using(GdipSetCustomLineCapStrokeJoin);
	Using(GdipGetCustomLineCapStrokeJoin);
	Using(GdipSetCustomLineCapBaseCap);
	Using(GdipGetCustomLineCapBaseCap);
	Using(GdipSetCustomLineCapBaseInset);
	Using(GdipGetCustomLineCapBaseInset);
	Using(GdipSetCustomLineCapWidthScale);
	Using(GdipGetCustomLineCapWidthScale);
	Using(GdipCreateAdjustableArrowCap);
	Using(GdipSetAdjustableArrowCapHeight);
	Using(GdipGetAdjustableArrowCapHeight);
	Using(GdipSetAdjustableArrowCapWidth);
	Using(GdipGetAdjustableArrowCapWidth);
	Using(GdipSetAdjustableArrowCapMiddleInset);
	Using(GdipGetAdjustableArrowCapMiddleInset);
	Using(GdipSetAdjustableArrowCapFillState);
	Using(GdipGetAdjustableArrowCapFillState);
	Using(GdipLoadImageFromStream);
	Using(GdipLoadImageFromFile);
	Using(GdipLoadImageFromStreamICM);
	Using(GdipLoadImageFromFileICM);
	Using(GdipCloneImage);
	Using(GdipDisposeImage);
	Using(GdipSaveImageToFile);
	Using(GdipSaveImageToStream);
	Using(GdipSaveAdd);
	Using(GdipSaveAddImage);
	Using(GdipGetImageGraphicsContext);
	Using(GdipGetImageBounds);
	Using(GdipGetImageDimension);
	Using(GdipGetImageType);
	Using(GdipGetImageWidth);
	Using(GdipGetImageHeight);
	Using(GdipGetImageHorizontalResolution);
	Using(GdipGetImageVerticalResolution);
	Using(GdipGetImageFlags);
	Using(GdipGetImageRawFormat);
	Using(GdipGetImagePixelFormat);
	Using(GdipGetImageThumbnail);
	Using(GdipGetEncoderParameterListSize);
	Using(GdipGetEncoderParameterList);
	Using(GdipImageGetFrameDimensionsCount);
	Using(GdipImageGetFrameDimensionsList);
	Using(GdipImageGetFrameCount);
	Using(GdipImageSelectActiveFrame);
	Using(GdipImageRotateFlip);
	Using(GdipGetImagePalette);
	Using(GdipSetImagePalette);
	Using(GdipGetImagePaletteSize);
	Using(GdipGetPropertyCount);
	Using(GdipGetPropertyIdList);
	Using(GdipGetPropertyItemSize);
	Using(GdipGetPropertyItem);
	Using(GdipGetPropertySize);
	Using(GdipGetAllPropertyItems);
	Using(GdipRemovePropertyItem);
	Using(GdipSetPropertyItem);
	Using(GdipImageForceValidation);
	Using(GdipCreateBitmapFromStream);
	Using(GdipCreateBitmapFromFile);
	Using(GdipCreateBitmapFromStreamICM);
	Using(GdipCreateBitmapFromFileICM);
	Using(GdipCreateBitmapFromScan0);
	Using(GdipCreateBitmapFromGraphics);
	Using(GdipCreateBitmapFromDirectDrawSurface);
	Using(GdipCreateBitmapFromGdiDib);
	Using(GdipCreateBitmapFromHBITMAP);
	Using(GdipCreateHBITMAPFromBitmap);
	Using(GdipCreateBitmapFromHICON);
	Using(GdipCreateHICONFromBitmap);
	Using(GdipCreateBitmapFromResource);
	Using(GdipCloneBitmapArea);
	Using(GdipCloneBitmapAreaI);
	Using(GdipBitmapLockBits);
	Using(GdipBitmapUnlockBits);
	Using(GdipBitmapGetPixel);
	Using(GdipBitmapSetPixel);
	Using(GdipBitmapSetResolution);
	Using(GdipCreateImageAttributes);
	Using(GdipCloneImageAttributes);
	Using(GdipDisposeImageAttributes);
	Using(GdipSetImageAttributesToIdentity);
	Using(GdipResetImageAttributes);
	Using(GdipSetImageAttributesColorMatrix);
	Using(GdipSetImageAttributesThreshold);
	Using(GdipSetImageAttributesGamma);
	Using(GdipSetImageAttributesNoOp);
	Using(GdipSetImageAttributesColorKeys);
	Using(GdipSetImageAttributesOutputChannel);
	Using(GdipSetImageAttributesOutputChannelColorProfile);
	Using(GdipSetImageAttributesRemapTable);
	Using(GdipSetImageAttributesWrapMode);
//	Using(GdipSetImageAttributesICMMode); // Fail to get this function pointer from gdiplus
	Using(GdipGetImageAttributesAdjustedPalette);
	Using(GdipFlush);
	Using(GdipCreateFromHDC);
	Using(GdipCreateFromHDC2);
	Using(GdipCreateFromHWND);
	Using(GdipCreateFromHWNDICM);
	Using(GdipDeleteGraphics);
	Using(GdipGetDC);
	Using(GdipReleaseDC);
	Using(GdipSetCompositingMode);
	Using(GdipGetCompositingMode);
	Using(GdipSetRenderingOrigin);
	Using(GdipGetRenderingOrigin);
	Using(GdipSetCompositingQuality);
	Using(GdipGetCompositingQuality);
	Using(GdipSetSmoothingMode);
	Using(GdipGetSmoothingMode);
	Using(GdipSetPixelOffsetMode);
	Using(GdipGetPixelOffsetMode);
	Using(GdipSetTextRenderingHint);
	Using(GdipGetTextRenderingHint);
	Using(GdipSetTextContrast);
	Using(GdipGetTextContrast);
	Using(GdipSetInterpolationMode);
	Using(GdipGetInterpolationMode);
	Using(GdipSetWorldTransform);
	Using(GdipResetWorldTransform);
	Using(GdipMultiplyWorldTransform);
	Using(GdipTranslateWorldTransform);
	Using(GdipScaleWorldTransform);
	Using(GdipRotateWorldTransform);
	Using(GdipGetWorldTransform);
	Using(GdipResetPageTransform);
	Using(GdipGetPageUnit);
	Using(GdipGetPageScale);
	Using(GdipSetPageUnit);
	Using(GdipSetPageScale);
	Using(GdipGetDpiX);
	Using(GdipGetDpiY);
	Using(GdipTransformPoints);
	Using(GdipTransformPointsI);
	Using(GdipGetNearestColor);
	Using(GdipCreateHalftonePalette);
	Using(GdipDrawLine);
	Using(GdipDrawLineI);
	Using(GdipDrawLines);
	Using(GdipDrawLinesI);
	Using(GdipDrawArc);
	Using(GdipDrawArcI);
	Using(GdipDrawBezier);
	Using(GdipDrawBezierI);
	Using(GdipDrawBeziers);
	Using(GdipDrawBeziersI);
	Using(GdipDrawRectangle);
	Using(GdipDrawRectangleI);
	Using(GdipDrawRectangles);
	Using(GdipDrawRectanglesI);
	Using(GdipDrawEllipse);
	Using(GdipDrawEllipseI);
	Using(GdipDrawPie);
	Using(GdipDrawPieI);
	Using(GdipDrawPolygon);
	Using(GdipDrawPolygonI);
	Using(GdipDrawPath);
	Using(GdipDrawCurve);
	Using(GdipDrawCurveI);
	Using(GdipDrawCurve2);
	Using(GdipDrawCurve2I);
	Using(GdipDrawCurve3);
	Using(GdipDrawCurve3I);
	Using(GdipDrawClosedCurve);
	Using(GdipDrawClosedCurveI);
	Using(GdipDrawClosedCurve2);
	Using(GdipDrawClosedCurve2I);
	Using(GdipGraphicsClear);
	Using(GdipFillRectangle);
	Using(GdipFillRectangleI);
	Using(GdipFillRectangles);
	Using(GdipFillRectanglesI);
	Using(GdipFillPolygon);
	Using(GdipFillPolygonI);
	Using(GdipFillPolygon2);
	Using(GdipFillPolygon2I);
	Using(GdipFillEllipse);
	Using(GdipFillEllipseI);
	Using(GdipFillPie);
	Using(GdipFillPieI);
	Using(GdipFillPath);
	Using(GdipFillClosedCurve);
	Using(GdipFillClosedCurveI);
	Using(GdipFillClosedCurve2);
	Using(GdipFillClosedCurve2I);
	Using(GdipFillRegion);
	Using(GdipDrawImage);
	Using(GdipDrawImageI);
	Using(GdipDrawImageRect);
	Using(GdipDrawImageRectI);
	Using(GdipDrawImagePoints);
	Using(GdipDrawImagePointsI);
	Using(GdipDrawImagePointRect);
	Using(GdipDrawImagePointRectI);
	Using(GdipDrawImageRectRect);
	Using(GdipDrawImageRectRectI);
	Using(GdipDrawImagePointsRect);
	Using(GdipDrawImagePointsRectI);
	Using(GdipEnumerateMetafileDestPoint);
	Using(GdipEnumerateMetafileDestPointI);
	Using(GdipEnumerateMetafileDestRect);
	Using(GdipEnumerateMetafileDestRectI);
	Using(GdipEnumerateMetafileDestPoints);
	Using(GdipEnumerateMetafileDestPointsI);
	Using(GdipEnumerateMetafileSrcRectDestPoint);
	Using(GdipEnumerateMetafileSrcRectDestPointI);
	Using(GdipEnumerateMetafileSrcRectDestRect);
	Using(GdipEnumerateMetafileSrcRectDestRectI);
	Using(GdipEnumerateMetafileSrcRectDestPoints);
	Using(GdipEnumerateMetafileSrcRectDestPointsI);
	Using(GdipPlayMetafileRecord);
	Using(GdipSetClipGraphics);
	Using(GdipSetClipRect);
	Using(GdipSetClipRectI);
	Using(GdipSetClipPath);
	Using(GdipSetClipRegion);
	Using(GdipSetClipHrgn);
	Using(GdipResetClip);
	Using(GdipTranslateClip);
	Using(GdipTranslateClipI);
	Using(GdipGetClip);
	Using(GdipGetClipBounds);
	Using(GdipGetClipBoundsI);
	Using(GdipIsClipEmpty);
	Using(GdipGetVisibleClipBounds);
	Using(GdipGetVisibleClipBoundsI);
	Using(GdipIsVisibleClipEmpty);
	Using(GdipIsVisiblePoint);
	Using(GdipIsVisiblePointI);
	Using(GdipIsVisibleRect);
	Using(GdipIsVisibleRectI);
	Using(GdipSaveGraphics);
	Using(GdipRestoreGraphics);
	Using(GdipBeginContainer);
	Using(GdipBeginContainerI);
	Using(GdipBeginContainer2);
	Using(GdipEndContainer);
	Using(GdipGetMetafileHeaderFromWmf);
	Using(GdipGetMetafileHeaderFromEmf);
	Using(GdipGetMetafileHeaderFromFile);
	Using(GdipGetMetafileHeaderFromStream);
	Using(GdipGetMetafileHeaderFromMetafile);
	Using(GdipGetHemfFromMetafile);
	Using(GdipCreateStreamOnFile);
	Using(GdipCreateMetafileFromWmf);
	Using(GdipCreateMetafileFromEmf);
	Using(GdipCreateMetafileFromFile);
	Using(GdipCreateMetafileFromWmfFile);
	Using(GdipCreateMetafileFromStream);
	Using(GdipRecordMetafile);
	Using(GdipRecordMetafileI);
	Using(GdipRecordMetafileFileName);
	Using(GdipRecordMetafileFileNameI);
	Using(GdipRecordMetafileStream);
	Using(GdipRecordMetafileStreamI);
	Using(GdipSetMetafileDownLevelRasterizationLimit);
	Using(GdipGetMetafileDownLevelRasterizationLimit);
	Using(GdipGetImageDecodersSize);
	Using(GdipGetImageDecoders);
	Using(GdipGetImageEncodersSize);
	Using(GdipGetImageEncoders);
	Using(GdipComment);
	Using(GdipCreateFontFamilyFromName);
	Using(GdipDeleteFontFamily);
	Using(GdipCloneFontFamily);
	Using(GdipGetGenericFontFamilySansSerif);
	Using(GdipGetGenericFontFamilySerif);
	Using(GdipGetGenericFontFamilyMonospace);
	Using(GdipGetFamilyName);
	Using(GdipIsStyleAvailable);
//	Using(GdipFontCollectionEnumerable); // Fail to get this function pointer from gdiplus
//	Using(GdipFontCollectionEnumerate); // Fail to get this function pointer from gdiplus
	Using(GdipGetEmHeight);
	Using(GdipGetCellAscent);
	Using(GdipGetCellDescent);
	Using(GdipGetLineSpacing);
	Using(GdipCreateFontFromDC);
	Using(GdipCreateFontFromLogfontA);
	Using(GdipCreateFontFromLogfontW);
	Using(GdipCreateFont);
	Using(GdipCloneFont);
	Using(GdipDeleteFont);
	Using(GdipGetFamily);
	Using(GdipGetFontStyle);
	Using(GdipGetFontSize);
	Using(GdipGetFontUnit);
	Using(GdipGetFontHeight);
	Using(GdipGetFontHeightGivenDPI);
	Using(GdipGetLogFontA);
	Using(GdipGetLogFontW);
	Using(GdipNewInstalledFontCollection);
	Using(GdipNewPrivateFontCollection);
	Using(GdipDeletePrivateFontCollection);
	Using(GdipGetFontCollectionFamilyCount);
	Using(GdipGetFontCollectionFamilyList);
	Using(GdipPrivateAddFontFile);
	Using(GdipPrivateAddMemoryFont);
	Using(GdipDrawString);
	Using(GdipMeasureString);
	Using(GdipMeasureCharacterRanges);
	Using(GdipDrawDriverString);
	Using(GdipMeasureDriverString);
	Using(GdipCreateStringFormat);
	Using(GdipStringFormatGetGenericDefault);
	Using(GdipStringFormatGetGenericTypographic);
	Using(GdipDeleteStringFormat);
	Using(GdipCloneStringFormat);
	Using(GdipSetStringFormatFlags);
	Using(GdipGetStringFormatFlags);
	Using(GdipSetStringFormatAlign);
	Using(GdipGetStringFormatAlign);
	Using(GdipSetStringFormatLineAlign);
	Using(GdipGetStringFormatLineAlign);
	Using(GdipSetStringFormatTrimming);
	Using(GdipGetStringFormatTrimming);
	Using(GdipSetStringFormatHotkeyPrefix);
	Using(GdipGetStringFormatHotkeyPrefix);
	Using(GdipSetStringFormatTabStops);
	Using(GdipGetStringFormatTabStops);
	Using(GdipGetStringFormatTabStopCount);
	Using(GdipSetStringFormatDigitSubstitution);
	Using(GdipGetStringFormatDigitSubstitution);
	Using(GdipGetStringFormatMeasurableCharacterRangeCount);
	Using(GdipSetStringFormatMeasurableCharacterRanges);
	Using(GdipCreateCachedBitmap);
	Using(GdipDeleteCachedBitmap);
	Using(GdipDrawCachedBitmap);
	Using(GdipEmfToWmfBits);
	Using(GdipSetImageAttributesCachedBackground);
	Using(GdipTestControl);
	Using(GdiplusNotificationHook);
	Using(GdiplusNotificationUnhook);

	return true;
}

typedef GpStatus (WINAPI *PFNGdiplusStartup)(ULONG_PTR* token, const GdiplusStartupInput* input, GdiplusStartupOutput* output);
typedef void(WINAPI *PFNGdiplusShutdown)(ULONG_PTR token);

//////////////////////////////////////////////////////////////////////
CGdip::CGdip()
{
	// Keep track of the number of instances
	m_iRefCount++;

	if (!m_hGdiPlusLib && !m_bInitFailed)
	{
		m_hGdiPlusLib = ::LoadLibrary("gdiplus.dll");
		m_bInitFailed = (m_hGdiPlusLib == NULL);
		if (m_bInitFailed)
			return;

		UsingList();
	}

	// See if we have already started
	if (m_dwToken)
		return;

	// Startup GDI+
	GdiplusStartupInput input;
	GdiplusStartupOutput output;
	DeclareUsing(GdiplusStartup);
	if (!GdiplusStartup || GdiplusStartup(&m_dwToken, &input, &output) != Ok)
	{
		m_bInitFailed = true;
		m_dwToken = 0;
		return;
	}
}

//////////////////////////////////////////////////////////////////////
CGdip::~CGdip()
{
	if (m_iRefCount <= 0)
		return;

	// Only the last instance releases the library
	if (--m_iRefCount != 0)
		return;

	if (m_dwToken)
	{
		// Shutdown GDI+
		DeclareUsing(GdiplusShutdown);
		if (GdiplusShutdown) GdiplusShutdown(m_dwToken);
		m_dwToken = 0;
	}

	if (m_hGdiPlusLib)
	{
		::FreeLibrary(m_hGdiPlusLib);
		m_hGdiPlusLib = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
bool CGdip::IsOK()
{
	return (m_hGdiPlusLib != NULL);
}
