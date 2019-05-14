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
TDP_DECLARE_ID(                    loadFilesSID,                       "Load files")
TDP_DECLARE_ID(                    saveFilesSID,                       "Save files")
TDP_DECLARE_ID(                   edgeDetectSID,                      "Edge detect")
TDP_DECLARE_ID(          signedDistanceFieldSID,            "Signed distance field")
TDP_DECLARE_ID(                       toGraySID,                          "To gray")
TDP_DECLARE_ID(                      toFloatSID,                         "To float")
TDP_DECLARE_ID(                        toHueSID,                           "To hue")
TDP_DECLARE_ID(                       toMonoSID,                          "To mono")
TDP_DECLARE_ID(                      deNoiseSID,                         "De noise")
TDP_DECLARE_ID(                        scaleSID,                            "Scale")
TDP_DECLARE_ID(                   noiseFieldSID,                      "Noise field")
TDP_DECLARE_ID(                 reduceColorsSID,                    "Reduce colors")
TDP_DECLARE_ID(                findPixelGridSID,                  "Find pixel grid")
TDP_DECLARE_ID(          normalizeBrightnessSID,             "Normalize brightness")
TDP_DECLARE_ID(                  extractRectSID,                     "Extract rect")
TDP_DECLARE_ID(              extractPolygonsSID,                 "Extract polygons")
TDP_DECLARE_ID(                   findShapesSID,                      "Find shapes")
TDP_DECLARE_ID(                     finalizeSID,                         "Finalize")
TDP_DECLARE_ID(                    addBorderSID,                       "Add border")
TDP_DECLARE_ID(                      bitwiseSID,                          "Bitwise")
TDP_DECLARE_ID(                   colorImageSID,                      "Color image")
TDP_DECLARE_ID(                    monoImageSID,                       "Mono image")
TDP_DECLARE_ID(                    grayImageSID,                       "Gray image")
TDP_DECLARE_ID(                     functionSID,                         "Function")
TDP_DECLARE_ID(             destinationWidthSID,                "Destination width")
TDP_DECLARE_ID(            destinationHeightSID,               "Destination height")
TDP_DECLARE_ID(              destinationSizeSID,                 "Destination size")
TDP_DECLARE_ID(              sizeCalculationSID,                 "Size calculation")
TDP_DECLARE_ID(                   originModeSID,                      "Origin mode")
TDP_DECLARE_ID(                  cellSegmentSID,                     "Cell segment")
TDP_DECLARE_ID(                minimumRadiusSID,                   "Minimum radius")
TDP_DECLARE_ID(          distanceFieldRadiusSID,            "Distance field radius")
TDP_DECLARE_ID(          maximumInitialCellsSID,            "Maximum initial cells")
TDP_DECLARE_ID(              growCellsPassesSID,                "Grow cells passes")
TDP_DECLARE_ID(                     colorizeSID,                         "Colorize")
TDP_DECLARE_ID(                     slotFillSID,                        "Slot fill")
TDP_DECLARE_ID(                        solidSID,                            "Solid")
TDP_DECLARE_ID(                         slotSID,                             "Slot")
TDP_DECLARE_ID(                   startAngleSID,                      "Start angle")
TDP_DECLARE_ID(                     maxAngleSID,                        "Max angle")
TDP_DECLARE_ID(                    stepAngleSID,                       "Step angle")
TDP_DECLARE_ID(                  labelsImageSID,                     "Labels image")
TDP_DECLARE_ID(                         maskSID,                             "Mask")
TDP_DECLARE_ID(                        colorSID,                            "Color")
TDP_DECLARE_ID(                        valueSID,                            "Value")
TDP_DECLARE_ID(                     drawMaskSID,                        "Draw mask")
TDP_DECLARE_ID(                     growModeSID,                        "Grow mode")
TDP_DECLARE_ID(               minAspectRatioSID,                 "Min aspect ratio")
TDP_DECLARE_ID(               maxAspectRatioSID,                 "Max aspect ratio")
TDP_DECLARE_ID(                   minDensitySID,                      "Min density")
TDP_DECLARE_ID(                   maxDensitySID,                      "Max density")
TDP_DECLARE_ID(                      minSizeSID,                         "Min size")
TDP_DECLARE_ID(                      maxSizeSID,                         "Max size")
TDP_DECLARE_ID(               noiseThresholdSID,                  "Noise threshold")
TDP_DECLARE_ID(             initialCoordModeSID,               "Initial coord mode")
TDP_DECLARE_ID(                         modeSID,                             "Mode")
TDP_DECLARE_ID(                   cornerModeSID,                      "Corner mode")
TDP_DECLARE_ID(              parametersOrderSID,                 "Parameters order")
TDP_DECLARE_ID(                 knobletWidthSID,                    "Knoblet width")
TDP_DECLARE_ID(                   endMinEachSID,                     "End min each")
TDP_DECLARE_ID(                 endMinEitherSID,                   "End min either")
TDP_DECLARE_ID(                    endMinSumSID,                      "End min sum")
TDP_DECLARE_ID(                borderMinEachSID,                  "Border min each")
TDP_DECLARE_ID(              borderMinEitherSID,                "Border min either")
TDP_DECLARE_ID(                 borderMinSumSID,                   "Border min sum")
TDP_DECLARE_ID(                   drawShapesSID,                      "Draw shapes")
TDP_DECLARE_ID(                    minPointsSID,                       "Min points")
TDP_DECLARE_ID(                 maxDeviationSID,                    "Max deviation")
TDP_DECLARE_ID(             maxJointDistanceSID,               "Max joint distance")
TDP_DECLARE_ID(                    shapeTypeSID,                       "Shape type")
TDP_DECLARE_ID(               angleDeviationSID,                  "Angle deviation")
TDP_DECLARE_ID(                       sourceSID,                           "Source")
TDP_DECLARE_ID(               drawDebugImageSID,                 "Draw debug image")
TDP_DECLARE_ID(                  loadAndSaveSID,                    "Load and save")
TDP_DECLARE_ID(                   conversionSID,                       "Conversion")
TDP_DECLARE_ID(                   processingSID,                       "Processing")
TDP_DECLARE_ID(                      drawingSID,                          "Drawing")
TDP_DECLARE_ID(               findAndSegmentSID,                 "Find and segment")
TDP_DECLARE_ID(                      toPolarSID,                         "To polar")
TDP_DECLARE_ID(                        widthSID,                            "Width")
TDP_DECLARE_ID(                       heightSID,                           "Height")
TDP_DECLARE_ID(                       radiusSID,                           "Radius")
TDP_DECLARE_ID(                  needleImageSID,                     "Needle image")
TDP_DECLARE_ID(                 externalNameSID,                    "External name")
TDP_DECLARE_ID(              fillConcaveHullSID,                "Fill concave hull")
TDP_DECLARE_ID(               colorThresholdSID,                  "Color threshold")
TDP_DECLARE_ID(                grayThresholdSID,                   "Gray threshold")
TDP_DECLARE_ID(                monoThresholdSID,                   "Mono threshold")
TDP_DECLARE_ID(                   gridSourceSID,                      "Grid source")
TDP_DECLARE_ID(                 clippingAreaSID,                    "Clipping area")
TDP_DECLARE_ID(                 clippingGridSID,                    "Clipping grid")
TDP_DECLARE_ID(                        linesSID,                            "Lines")
TDP_DECLARE_ID(                         gridSID,                             "Grid")
TDP_DECLARE_ID(                  channelModeSID,                     "Channel mode")
TDP_DECLARE_ID(                 channelOrderSID,                    "Channel order")
TDP_DECLARE_ID(            pixelManipulationSID,               "Pixel manipulation")
TDP_DECLARE_ID(                      calcRedSID,                         "Calc red")
TDP_DECLARE_ID(                    calcGreenSID,                       "Calc green")
TDP_DECLARE_ID(                     calcBlueSID,                        "Calc blue")
TDP_DECLARE_ID(                    calcAlphaSID,                       "Calc alpha")
TDP_DECLARE_ID(                     calcByteSID,                        "Calc byte")
TDP_DECLARE_ID(                            xSID,                                "X")
TDP_DECLARE_ID(                            ySID,                                "Y")

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
