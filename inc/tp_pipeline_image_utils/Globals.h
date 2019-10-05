#ifndef tp_pipeline_image_utils_Globals_h
#define tp_pipeline_image_utils_Globals_h

#include "tp_utils/TPPixel.h"

#include "tp_utils/StringID.h"

#include <unordered_map>

namespace tp_data
{
class CollectionFactory;
}

namespace tp_pipeline
{
class StepDelegateMap;
class StepDetails;
struct Parameter;
}

//##################################################################################################
//! Pipeline step delegates for processing images using tp_image_utils
namespace tp_pipeline_image_utils
{
TP_DECLARE_ID(                    loadFilesSID,                       "Load files");
TP_DECLARE_ID(                    saveFilesSID,                       "Save files");
TP_DECLARE_ID(                   edgeDetectSID,                      "Edge detect");
TP_DECLARE_ID(          signedDistanceFieldSID,            "Signed distance field");
TP_DECLARE_ID(                       toGraySID,                          "To gray");
TP_DECLARE_ID(                      toFloatSID,                         "To float");
TP_DECLARE_ID(                        toHueSID,                           "To hue");
TP_DECLARE_ID(                       toMonoSID,                          "To mono");
TP_DECLARE_ID(                      deNoiseSID,                         "De noise");
TP_DECLARE_ID(                        scaleSID,                            "Scale");
TP_DECLARE_ID(                   noiseFieldSID,                      "Noise field");
TP_DECLARE_ID(                 reduceColorsSID,                    "Reduce colors");
TP_DECLARE_ID(                findPixelGridSID,                  "Find pixel grid");
TP_DECLARE_ID(          normalizeBrightnessSID,             "Normalize brightness");
TP_DECLARE_ID(                  extractRectSID,                     "Extract rect");
TP_DECLARE_ID(              extractPolygonsSID,                 "Extract polygons");
TP_DECLARE_ID(                   findShapesSID,                      "Find shapes");
TP_DECLARE_ID(                     finalizeSID,                         "Finalize");
TP_DECLARE_ID(                    addBorderSID,                       "Add border");
TP_DECLARE_ID(                      bitwiseSID,                          "Bitwise");
TP_DECLARE_ID(                   colorImageSID,                      "Color image");
TP_DECLARE_ID(                    monoImageSID,                       "Mono image");
TP_DECLARE_ID(                    grayImageSID,                       "Gray image");
TP_DECLARE_ID(                     functionSID,                         "Function");
TP_DECLARE_ID(             destinationWidthSID,                "Destination width");
TP_DECLARE_ID(            destinationHeightSID,               "Destination height");
TP_DECLARE_ID(              destinationSizeSID,                 "Destination size");
TP_DECLARE_ID(              sizeCalculationSID,                 "Size calculation");
TP_DECLARE_ID(                   originModeSID,                      "Origin mode");
TP_DECLARE_ID(                  cellSegmentSID,                     "Cell segment");
TP_DECLARE_ID(                minimumRadiusSID,                   "Minimum radius");
TP_DECLARE_ID(          distanceFieldRadiusSID,            "Distance field radius");
TP_DECLARE_ID(          maximumInitialCellsSID,            "Maximum initial cells");
TP_DECLARE_ID(              growCellsPassesSID,                "Grow cells passes");
TP_DECLARE_ID(                     colorizeSID,                         "Colorize");
TP_DECLARE_ID(                     slotFillSID,                        "Slot fill");
TP_DECLARE_ID(                        solidSID,                            "Solid");
TP_DECLARE_ID(                         slotSID,                             "Slot");
TP_DECLARE_ID(                   startAngleSID,                      "Start angle");
TP_DECLARE_ID(                     maxAngleSID,                        "Max angle");
TP_DECLARE_ID(                    stepAngleSID,                       "Step angle");
TP_DECLARE_ID(                  labelsImageSID,                     "Labels image");
TP_DECLARE_ID(                         maskSID,                             "Mask");
TP_DECLARE_ID(                        colorSID,                            "Color");
TP_DECLARE_ID(                        valueSID,                            "Value");
TP_DECLARE_ID(                     drawMaskSID,                        "Draw mask");
TP_DECLARE_ID(                     growModeSID,                        "Grow mode");
TP_DECLARE_ID(               minAspectRatioSID,                 "Min aspect ratio");
TP_DECLARE_ID(               maxAspectRatioSID,                 "Max aspect ratio");
TP_DECLARE_ID(                   minDensitySID,                      "Min density");
TP_DECLARE_ID(                   maxDensitySID,                      "Max density");
TP_DECLARE_ID(                      minSizeSID,                         "Min size");
TP_DECLARE_ID(                      maxSizeSID,                         "Max size");
TP_DECLARE_ID(               noiseThresholdSID,                  "Noise threshold");
TP_DECLARE_ID(             initialCoordModeSID,               "Initial coord mode");
TP_DECLARE_ID(                         modeSID,                             "Mode");
TP_DECLARE_ID(                   cornerModeSID,                      "Corner mode");
TP_DECLARE_ID(              parametersOrderSID,                 "Parameters order");
TP_DECLARE_ID(                 knobletWidthSID,                    "Knoblet width");
TP_DECLARE_ID(                   endMinEachSID,                     "End min each");
TP_DECLARE_ID(                 endMinEitherSID,                   "End min either");
TP_DECLARE_ID(                    endMinSumSID,                      "End min sum");
TP_DECLARE_ID(                borderMinEachSID,                  "Border min each");
TP_DECLARE_ID(              borderMinEitherSID,                "Border min either");
TP_DECLARE_ID(                 borderMinSumSID,                   "Border min sum");
TP_DECLARE_ID(                   drawShapesSID,                      "Draw shapes");
TP_DECLARE_ID(                    minPointsSID,                       "Min points");
TP_DECLARE_ID(                 maxDeviationSID,                    "Max deviation");
TP_DECLARE_ID(             maxJointDistanceSID,               "Max joint distance");
TP_DECLARE_ID(                    shapeTypeSID,                       "Shape type");
TP_DECLARE_ID(               angleDeviationSID,                  "Angle deviation");
TP_DECLARE_ID(                       sourceSID,                           "Source");
TP_DECLARE_ID(               drawDebugImageSID,                 "Draw debug image");
TP_DECLARE_ID(                  loadAndSaveSID,                    "Load and save");
TP_DECLARE_ID(                   conversionSID,                       "Conversion");
TP_DECLARE_ID(                   processingSID,                       "Processing");
TP_DECLARE_ID(                      drawingSID,                          "Drawing");
TP_DECLARE_ID(               findAndSegmentSID,                 "Find and segment");
TP_DECLARE_ID(                      toPolarSID,                         "To polar");
TP_DECLARE_ID(                        widthSID,                            "Width");
TP_DECLARE_ID(                       heightSID,                           "Height");
TP_DECLARE_ID(                       radiusSID,                           "Radius");
TP_DECLARE_ID(                  needleImageSID,                     "Needle image");
TP_DECLARE_ID(                 externalNameSID,                    "External name");
TP_DECLARE_ID(              fillConcaveHullSID,                "Fill concave hull");
TP_DECLARE_ID(               colorThresholdSID,                  "Color threshold");
TP_DECLARE_ID(                grayThresholdSID,                   "Gray threshold");
TP_DECLARE_ID(                monoThresholdSID,                   "Mono threshold");
TP_DECLARE_ID(                   gridSourceSID,                      "Grid source");
TP_DECLARE_ID(                 clippingAreaSID,                    "Clipping area");
TP_DECLARE_ID(                 clippingGridSID,                    "Clipping grid");
TP_DECLARE_ID(                        linesSID,                            "Lines");
TP_DECLARE_ID(                         gridSID,                             "Grid");
TP_DECLARE_ID(                  channelModeSID,                     "Channel mode");
TP_DECLARE_ID(                 channelOrderSID,                    "Channel order");
TP_DECLARE_ID(            pixelManipulationSID,               "Pixel manipulation");
TP_DECLARE_ID(                      calcRedSID,                         "Calc red");
TP_DECLARE_ID(                    calcGreenSID,                       "Calc green");
TP_DECLARE_ID(                     calcBlueSID,                        "Calc blue");
TP_DECLARE_ID(                    calcAlphaSID,                       "Calc alpha");
TP_DECLARE_ID(                     calcByteSID,                        "Calc byte");
TP_DECLARE_ID(                            xSID,                                "X");
TP_DECLARE_ID(                            ySID,                                "Y");
TP_DECLARE_ID(                       kernelSID,                           "Kernel");

//##################################################################################################
//! Add the step delegates that this module provides to the StepDelegateMap
void createStepDelegates(tp_pipeline::StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory);

//##################################################################################################
TPPixel makeColor(size_t index);

//##################################################################################################
void validateColor(tp_pipeline::Parameter& param, const TPPixel& color);

//##################################################################################################
//! Static initialization of this module, see TP_STATIC_INIT in dependencies.pri
int staticInit();

}

#endif
