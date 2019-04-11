TARGET = tp_pipeline_image_utils
TEMPLATE = lib

DEFINES += TP_PIPELINE_IMAGE_UTILS_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_pipeline_image_utils/Globals.h

#-- Delegates --------------------------------------------------------------------------------------
SOURCES += src/step_delegates/LoadFilesStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/LoadFilesStepDelegate.h

SOURCES += src/step_delegates/EdgeDetectStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/EdgeDetectStepDelegate.h

SOURCES += src/step_delegates/SignedDistanceFieldStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/SignedDistanceFieldStepDelegate.h

SOURCES += src/step_delegates/ToGrayStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ToGrayStepDelegate.h

SOURCES += src/step_delegates/ToFloatStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ToFloatStepDelegate.h

SOURCES += src/step_delegates/ToHueStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ToHueStepDelegate.h

SOURCES += src/step_delegates/ToMonoStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ToMonoStepDelegate.h

SOURCES += src/step_delegates/DeNoiseStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/DeNoiseStepDelegate.h

SOURCES += src/step_delegates/ScaleStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ScaleStepDelegate.h

SOURCES += src/step_delegates/NoiseFieldStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/NoiseFieldStepDelegate.h

SOURCES += src/step_delegates/ConvolutionMatrixStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ConvolutionMatrixStepDelegate.h

SOURCES += src/step_delegates/ReduceColorsStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ReduceColorsStepDelegate.h

SOURCES += src/step_delegates/FindPixelGridStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/FindPixelGridStepDelegate.h

SOURCES += src/step_delegates/NormalizeBrightnessStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/NormalizeBrightnessStepDelegate.h

SOURCES += src/step_delegates/ExtractRectStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ExtractRectStepDelegate.h

SOURCES += src/step_delegates/FindShapesStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/FindShapesStepDelegate.h

SOURCES += src/step_delegates/FinalizeStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/FinalizeStepDelegate.h

SOURCES += src/step_delegates/AddBorderStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/AddBorderStepDelegate.h

SOURCES += src/step_delegates/BitwiseStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/BitwiseStepDelegate.h

SOURCES += src/step_delegates/CellSegmentStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/CellSegmentStepDelegate.h

SOURCES += src/step_delegates/ColorizeStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ColorizeStepDelegate.h

SOURCES += src/step_delegates/SlotFillStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/SlotFillStepDelegate.h

SOURCES += src/step_delegates/DrawMaskStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/DrawMaskStepDelegate.h

SOURCES += src/step_delegates/DrawShapesStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/DrawShapesStepDelegate.h

SOURCES += src/step_delegates/ToPolarStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ToPolarStepDelegate.h

SOURCES += src/step_delegates/FillConcaveHullStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/FillConcaveHullStepDelegate.h

SOURCES += src/step_delegates/ExtractPolygonsStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/ExtractPolygonsStepDelegate.h

SOURCES += src/step_delegates/PixelManipulationStepDelegate.cpp
HEADERS += inc/tp_pipeline_image_utils/step_delegates/PixelManipulationStepDelegate.h

