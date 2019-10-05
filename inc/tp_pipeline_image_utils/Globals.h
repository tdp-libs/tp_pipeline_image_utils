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
tp_qt_DECLARE_ID(                    loadFilesSID,                       "Load files");
tp_qt_DECLARE_ID(                    saveFilesSID,                       "Save files");
tp_qt_DECLARE_ID(                   edgeDetectSID,                      "Edge detect");
tp_qt_DECLARE_ID(          signedDistanceFieldSID,            "Signed distance field");
tp_qt_DECLARE_ID(                       toGraySID,                          "To gray");
tp_qt_DECLARE_ID(                      toFloatSID,                         "To float");
tp_qt_DECLARE_ID(                        toHueSID,                           "To hue");
tp_qt_DECLARE_ID(                       toMonoSID,                          "To mono");
tp_qt_DECLARE_ID(                      deNoiseSID,                         "De noise");
tp_qt_DECLARE_ID(                        scaleSID,                            "Scale");
tp_qt_DECLARE_ID(                   noiseFieldSID,                      "Noise field");
tp_qt_DECLARE_ID(                 reduceColorsSID,                    "Reduce colors");
tp_qt_DECLARE_ID(                findPixelGridSID,                  "Find pixel grid");
tp_qt_DECLARE_ID(          normalizeBrightnessSID,             "Normalize brightness");
tp_qt_DECLARE_ID(                  extractRectSID,                     "Extract rect");
tp_qt_DECLARE_ID(              extractPolygonsSID,                 "Extract polygons");
tp_qt_DECLARE_ID(                   findShapesSID,                      "Find shapes");
tp_qt_DECLARE_ID(                     finalizeSID,                         "Finalize");
tp_qt_DECLARE_ID(                    addBorderSID,                       "Add border");
tp_qt_DECLARE_ID(                      bitwiseSID,                          "Bitwise");
tp_qt_DECLARE_ID(                   colorImageSID,                      "Color image");
tp_qt_DECLARE_ID(                    monoImageSID,                       "Mono image");
tp_qt_DECLARE_ID(                    grayImageSID,                       "Gray image");
tp_qt_DECLARE_ID(                     functionSID,                         "Function");
tp_qt_DECLARE_ID(             destinationWidthSID,                "Destination width");
tp_qt_DECLARE_ID(            destinationHeightSID,               "Destination height");
tp_qt_DECLARE_ID(              destinationSizeSID,                 "Destination size");
tp_qt_DECLARE_ID(              sizeCalculationSID,                 "Size calculation");
tp_qt_DECLARE_ID(                   originModeSID,                      "Origin mode");
tp_qt_DECLARE_ID(                  cellSegmentSID,                     "Cell segment");
tp_qt_DECLARE_ID(                minimumRadiusSID,                   "Minimum radius");
tp_qt_DECLARE_ID(          distanceFieldRadiusSID,            "Distance field radius");
tp_qt_DECLARE_ID(          maximumInitialCellsSID,            "Maximum initial cells");
tp_qt_DECLARE_ID(              growCellsPassesSID,                "Grow cells passes");
tp_qt_DECLARE_ID(                     colorizeSID,                         "Colorize");
tp_qt_DECLARE_ID(                     slotFillSID,                        "Slot fill");
tp_qt_DECLARE_ID(                        solidSID,                            "Solid");
tp_qt_DECLARE_ID(                         slotSID,                             "Slot");
tp_qt_DECLARE_ID(                   startAngleSID,                      "Start angle");
tp_qt_DECLARE_ID(                     maxAngleSID,                        "Max angle");
tp_qt_DECLARE_ID(                    stepAngleSID,                       "Step angle");
tp_qt_DECLARE_ID(                  labelsImageSID,                     "Labels image");
tp_qt_DECLARE_ID(                         maskSID,                             "Mask");
tp_qt_DECLARE_ID(                        colorSID,                            "Color");
tp_qt_DECLARE_ID(                        valueSID,                            "Value");
tp_qt_DECLARE_ID(                     drawMaskSID,                        "Draw mask");
tp_qt_DECLARE_ID(                     growModeSID,                        "Grow mode");
tp_qt_DECLARE_ID(               minAspectRatioSID,                 "Min aspect ratio");
tp_qt_DECLARE_ID(               maxAspectRatioSID,                 "Max aspect ratio");
tp_qt_DECLARE_ID(                   minDensitySID,                      "Min density");
tp_qt_DECLARE_ID(                   maxDensitySID,                      "Max density");
tp_qt_DECLARE_ID(                      minSizeSID,                         "Min size");
tp_qt_DECLARE_ID(                      maxSizeSID,                         "Max size");
tp_qt_DECLARE_ID(               noiseThresholdSID,                  "Noise threshold");
tp_qt_DECLARE_ID(             initialCoordModeSID,               "Initial coord mode");
tp_qt_DECLARE_ID(                         modeSID,                             "Mode");
tp_qt_DECLARE_ID(                   cornerModeSID,                      "Corner mode");
tp_qt_DECLARE_ID(              parametersOrderSID,                 "Parameters order");
tp_qt_DECLARE_ID(                 knobletWidthSID,                    "Knoblet width");
tp_qt_DECLARE_ID(                   endMinEachSID,                     "End min each");
tp_qt_DECLARE_ID(                 endMinEitherSID,                   "End min either");
tp_qt_DECLARE_ID(                    endMinSumSID,                      "End min sum");
tp_qt_DECLARE_ID(                borderMinEachSID,                  "Border min each");
tp_qt_DECLARE_ID(              borderMinEitherSID,                "Border min either");
tp_qt_DECLARE_ID(                 borderMinSumSID,                   "Border min sum");
tp_qt_DECLARE_ID(                   drawShapesSID,                      "Draw shapes");
tp_qt_DECLARE_ID(                    minPointsSID,                       "Min points");
tp_qt_DECLARE_ID(                 maxDeviationSID,                    "Max deviation");
tp_qt_DECLARE_ID(             maxJointDistanceSID,               "Max joint distance");
tp_qt_DECLARE_ID(                    shapeTypeSID,                       "Shape type");
tp_qt_DECLARE_ID(               angleDeviationSID,                  "Angle deviation");
tp_qt_DECLARE_ID(                       sourceSID,                           "Source");
tp_qt_DECLARE_ID(               drawDebugImageSID,                 "Draw debug image");
tp_qt_DECLARE_ID(                  loadAndSaveSID,                    "Load and save");
tp_qt_DECLARE_ID(                   conversionSID,                       "Conversion");
tp_qt_DECLARE_ID(                   processingSID,                       "Processing");
tp_qt_DECLARE_ID(                      drawingSID,                          "Drawing");
tp_qt_DECLARE_ID(               findAndSegmentSID,                 "Find and segment");
tp_qt_DECLARE_ID(                      toPolarSID,                         "To polar");
tp_qt_DECLARE_ID(                        widthSID,                            "Width");
tp_qt_DECLARE_ID(                       heightSID,                           "Height");
tp_qt_DECLARE_ID(                       radiusSID,                           "Radius");
tp_qt_DECLARE_ID(                  needleImageSID,                     "Needle image");
tp_qt_DECLARE_ID(                 externalNameSID,                    "External name");
tp_qt_DECLARE_ID(              fillConcaveHullSID,                "Fill concave hull");
tp_qt_DECLARE_ID(               colorThresholdSID,                  "Color threshold");
tp_qt_DECLARE_ID(                grayThresholdSID,                   "Gray threshold");
tp_qt_DECLARE_ID(                monoThresholdSID,                   "Mono threshold");
tp_qt_DECLARE_ID(                   gridSourceSID,                      "Grid source");
tp_qt_DECLARE_ID(                 clippingAreaSID,                    "Clipping area");
tp_qt_DECLARE_ID(                 clippingGridSID,                    "Clipping grid");
tp_qt_DECLARE_ID(                        linesSID,                            "Lines");
tp_qt_DECLARE_ID(                         gridSID,                             "Grid");
tp_qt_DECLARE_ID(                  channelModeSID,                     "Channel mode");
tp_qt_DECLARE_ID(                 channelOrderSID,                    "Channel order");
tp_qt_DECLARE_ID(            pixelManipulationSID,               "Pixel manipulation");
tp_qt_DECLARE_ID(                      calcRedSID,                         "Calc red");
tp_qt_DECLARE_ID(                    calcGreenSID,                       "Calc green");
tp_qt_DECLARE_ID(                     calcBlueSID,                        "Calc blue");
tp_qt_DECLARE_ID(                    calcAlphaSID,                       "Calc alpha");
tp_qt_DECLARE_ID(                     calcByteSID,                        "Calc byte");
tp_qt_DECLARE_ID(                            xSID,                                "X");
tp_qt_DECLARE_ID(                            ySID,                                "Y");
tp_qt_DECLARE_ID(                       kernelSID,                           "Kernel");

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
