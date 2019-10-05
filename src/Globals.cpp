#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline_image_utils/step_delegates/LoadFilesStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/EdgeDetectStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/SignedDistanceFieldStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ToGrayStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ToFloatStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ToHueStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ToMonoStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/DeNoiseStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ScaleStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/NoiseFieldStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ConvolutionMatrixStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ReduceColorsStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/FindPixelGridStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/NormalizeBrightnessStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ExtractRectStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ExtractPolygonsStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/FindShapesStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/FinalizeStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/AddBorderStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/BitwiseStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/CellSegmentStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ColorizeStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/SlotFillStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/DrawMaskStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/DrawShapesStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/ToPolarStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/FillConcaveHullStepDelegate.h"
#include "tp_pipeline_image_utils/step_delegates/PixelManipulationStepDelegate.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepDelegateMap.h"

//##################################################################################################
namespace tp_pipeline_image_utils
{
TP_DEFINE_ID(                    loadFilesSID,                       "Load files");
TP_DEFINE_ID(                    saveFilesSID,                       "Save files");
TP_DEFINE_ID(                   edgeDetectSID,                      "Edge detect");
TP_DEFINE_ID(          signedDistanceFieldSID,            "Signed distance field");
TP_DEFINE_ID(                       toGraySID,                          "To gray");
TP_DEFINE_ID(                      toFloatSID,                         "To float");
TP_DEFINE_ID(                        toHueSID,                           "To hue");
TP_DEFINE_ID(                       toMonoSID,                          "To mono");
TP_DEFINE_ID(                      deNoiseSID,                         "De noise");
TP_DEFINE_ID(                        scaleSID,                            "Scale");
TP_DEFINE_ID(                   noiseFieldSID,                      "Noise field");
TP_DEFINE_ID(                 reduceColorsSID,                    "Reduce colors");
TP_DEFINE_ID(                findPixelGridSID,                  "Find pixel grid");
TP_DEFINE_ID(          normalizeBrightnessSID,             "Normalize brightness");
TP_DEFINE_ID(                  extractRectSID,                     "Extract rect");
TP_DEFINE_ID(              extractPolygonsSID,                 "Extract polygons");
TP_DEFINE_ID(                   findShapesSID,                      "Find shapes");
TP_DEFINE_ID(                     finalizeSID,                         "Finalize");
TP_DEFINE_ID(                    addBorderSID,                       "Add border");
TP_DEFINE_ID(                      bitwiseSID,                          "Bitwise");
TP_DEFINE_ID(                   colorImageSID,                      "Color image");
TP_DEFINE_ID(                    monoImageSID,                       "Mono image");
TP_DEFINE_ID(                    grayImageSID,                       "Gray image");
TP_DEFINE_ID(                     functionSID,                         "Function");
TP_DEFINE_ID(             destinationWidthSID,                "Destination width");
TP_DEFINE_ID(            destinationHeightSID,               "Destination height");
TP_DEFINE_ID(              destinationSizeSID,                 "Destination size");
TP_DEFINE_ID(              sizeCalculationSID,                 "Size calculation");
TP_DEFINE_ID(                   originModeSID,                      "Origin mode");
TP_DEFINE_ID(                  cellSegmentSID,                     "Cell segment");
TP_DEFINE_ID(                minimumRadiusSID,                   "Minimum radius");
TP_DEFINE_ID(          distanceFieldRadiusSID,            "Distance field radius");
TP_DEFINE_ID(          maximumInitialCellsSID,            "Maximum initial cells");
TP_DEFINE_ID(              growCellsPassesSID,                "Grow cells passes");
TP_DEFINE_ID(                     colorizeSID,                         "Colorize");
TP_DEFINE_ID(                     slotFillSID,                        "Slot fill");
TP_DEFINE_ID(                        solidSID,                            "Solid");
TP_DEFINE_ID(                         slotSID,                             "Slot");
TP_DEFINE_ID(                   startAngleSID,                      "Start angle");
TP_DEFINE_ID(                     maxAngleSID,                        "Max angle");
TP_DEFINE_ID(                    stepAngleSID,                       "Step angle");
TP_DEFINE_ID(                  labelsImageSID,                     "Labels image");
TP_DEFINE_ID(                         maskSID,                             "Mask");
TP_DEFINE_ID(                        colorSID,                            "Color");
TP_DEFINE_ID(                        valueSID,                            "Value");
TP_DEFINE_ID(                     drawMaskSID,                        "Draw mask");
TP_DEFINE_ID(                     growModeSID,                        "Grow mode");
TP_DEFINE_ID(               minAspectRatioSID,                 "Min aspect ratio");
TP_DEFINE_ID(               maxAspectRatioSID,                 "Max aspect ratio");
TP_DEFINE_ID(                   minDensitySID,                      "Min density");
TP_DEFINE_ID(                   maxDensitySID,                      "Max density");
TP_DEFINE_ID(                      minSizeSID,                         "Min size");
TP_DEFINE_ID(                      maxSizeSID,                         "Max size");
TP_DEFINE_ID(               noiseThresholdSID,                  "Noise threshold");
TP_DEFINE_ID(             initialCoordModeSID,               "Initial coord mode");
TP_DEFINE_ID(                         modeSID,                             "Mode");
TP_DEFINE_ID(                   cornerModeSID,                      "Corner mode");
TP_DEFINE_ID(              parametersOrderSID,                 "Parameters order");
TP_DEFINE_ID(                 knobletWidthSID,                    "Knoblet width");
TP_DEFINE_ID(                   endMinEachSID,                     "End min each");
TP_DEFINE_ID(                 endMinEitherSID,                   "End min either");
TP_DEFINE_ID(                    endMinSumSID,                      "End min sum");
TP_DEFINE_ID(                borderMinEachSID,                  "Border min each");
TP_DEFINE_ID(              borderMinEitherSID,                "Border min either");
TP_DEFINE_ID(                 borderMinSumSID,                   "Border min sum");
TP_DEFINE_ID(                   drawShapesSID,                      "Draw shapes");
TP_DEFINE_ID(                    minPointsSID,                       "Min points");
TP_DEFINE_ID(                 maxDeviationSID,                    "Max deviation");
TP_DEFINE_ID(             maxJointDistanceSID,               "Max joint distance");
TP_DEFINE_ID(                    shapeTypeSID,                       "Shape type");
TP_DEFINE_ID(               angleDeviationSID,                  "Angle deviation");
TP_DEFINE_ID(                       sourceSID,                           "Source");
TP_DEFINE_ID(               drawDebugImageSID,                 "Draw debug image");
TP_DEFINE_ID(                  loadAndSaveSID,                    "Load and save");
TP_DEFINE_ID(                   conversionSID,                       "Conversion");
TP_DEFINE_ID(                   processingSID,                       "Processing");
TP_DEFINE_ID(                      drawingSID,                          "Drawing");
TP_DEFINE_ID(               findAndSegmentSID,                 "Find and segment");
TP_DEFINE_ID(                      toPolarSID,                         "To polar");
TP_DEFINE_ID(                        widthSID,                            "Width");
TP_DEFINE_ID(                       heightSID,                           "Height");
TP_DEFINE_ID(                       radiusSID,                           "Radius");
TP_DEFINE_ID(                  needleImageSID,                     "Needle image");
TP_DEFINE_ID(                 externalNameSID,                    "External name");
TP_DEFINE_ID(              fillConcaveHullSID,                "Fill concave hull");
TP_DEFINE_ID(               colorThresholdSID,                  "Color threshold");
TP_DEFINE_ID(                grayThresholdSID,                   "Gray threshold");
TP_DEFINE_ID(                monoThresholdSID,                   "Mono threshold");
TP_DEFINE_ID(                   gridSourceSID,                      "Grid source");
TP_DEFINE_ID(                 clippingAreaSID,                    "Clipping area");
TP_DEFINE_ID(                 clippingGridSID,                    "Clipping grid");
TP_DEFINE_ID(                        linesSID,                            "Lines");
TP_DEFINE_ID(                         gridSID,                             "Grid");
TP_DEFINE_ID(                  channelModeSID,                     "Channel mode");
TP_DEFINE_ID(                 channelOrderSID,                    "Channel order");
TP_DEFINE_ID(            pixelManipulationSID,               "Pixel manipulation");
TP_DEFINE_ID(                      calcRedSID,                         "Calc red");
TP_DEFINE_ID(                    calcGreenSID,                       "Calc green");
TP_DEFINE_ID(                     calcBlueSID,                        "Calc blue");
TP_DEFINE_ID(                    calcAlphaSID,                       "Calc alpha");
TP_DEFINE_ID(                     calcByteSID,                        "Calc byte");
TP_DEFINE_ID(                            xSID,                                "X");
TP_DEFINE_ID(                            ySID,                                "Y");
TP_DEFINE_ID(                       kernelSID,                           "Kernel");

//##################################################################################################
void createStepDelegates(tp_pipeline::StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory)
{
  TP_UNUSED(collectionFactory);
  stepDelegates.addStepDelegate(new LoadFilesStepDelegate          );
  stepDelegates.addStepDelegate(new EdgeDetectStepDelegate         );
  stepDelegates.addStepDelegate(new SignedDistanceFieldStepDelegate);
  stepDelegates.addStepDelegate(new ToGrayStepDelegate             );
  stepDelegates.addStepDelegate(new ToFloatStepDelegate            );
  stepDelegates.addStepDelegate(new ToHueStepDelegate              );
  stepDelegates.addStepDelegate(new ToMonoStepDelegate             );
  stepDelegates.addStepDelegate(new DeNoiseStepDelegate            );
  stepDelegates.addStepDelegate(new ScaleStepDelegate              );
  stepDelegates.addStepDelegate(new NoiseFieldStepDelegate         );
  stepDelegates.addStepDelegate(new ConvolutionMatrixStepDelegate  );
  stepDelegates.addStepDelegate(new ReduceColorsStepDelegate       );
  stepDelegates.addStepDelegate(new FindPixelGridStepDelegate      );
  stepDelegates.addStepDelegate(new NormalizeBrightnessStepDelegate);
  stepDelegates.addStepDelegate(new ExtractRectStepDelegate        );
  stepDelegates.addStepDelegate(new ExtractPolygonsStepDelegate    );
  stepDelegates.addStepDelegate(new FindShapesStepDelegate         );
  stepDelegates.addStepDelegate(new FinalizeStepDelegate(collectionFactory));
  stepDelegates.addStepDelegate(new AddBorderStepDelegate          );
  stepDelegates.addStepDelegate(new BitwiseStepDelegate            );
  stepDelegates.addStepDelegate(new CellSegmentStepDelegate        );
  stepDelegates.addStepDelegate(new ColorizeStepDelegate           );
  stepDelegates.addStepDelegate(new SlotFillStepDelegate           );
  stepDelegates.addStepDelegate(new DrawMaskStepDelegate           );
  stepDelegates.addStepDelegate(new DrawShapesStepDelegate         );
  stepDelegates.addStepDelegate(new ToPolarStepDelegate            );
  stepDelegates.addStepDelegate(new FillConcaveHullStepDelegate    );
  stepDelegates.addStepDelegate(new PixelManipulationStepDelegate  );
}

//##################################################################################################
TPPixel makeColor(size_t index)
{
  static const TPPixel lookup[256] = {
    {255,  0,  0},{ 71, 17, 17},{201,204,  0},{162,163, 78},{  0,133,  4},{ 59,122, 61},{  0, 58, 61},{ 66, 91, 92},
    { 21, 17, 71},{245,  0,224},{184, 88,176},{245,  0,  0},{ 61, 15, 15},{171,173,  0},{131,133, 64},{  0,122,  4},
    { 54,112, 56},{ 54,216,224},{ 59, 80, 82},{ 18, 15, 61},{224,  0,206},{153, 73,146},{235,  0,  0},{ 51, 12, 12},
    {140,143,  0},{121,122, 59},{  0,112,  4},{ 49,102, 51},{ 42,167,173},{ 51, 70, 71},{ 15, 12, 51},{204,  0,187},
    {133, 64,127},{214,  0,  0},{255,122,122},{130,133,  0},{111,112, 54},{  0,102,  3},{ 44, 92, 46},{ 37,147,153},
    { 44, 60, 61},{131,122,255},{184,  0,168},{122, 59,117},{204,  0,  0},{235,113,113},{120,122,  0},{101,102, 49},
    {  0, 92,  3},{ 39, 82, 41},{ 32,128,133},{ 37, 50, 51},{121,113,235},{163,  0,150},{112, 54,107},{194,  0,  0},
    {214,103,103},{110,112,  0},{ 91, 92, 44},{  0, 82,  3},{ 34, 71, 36},{ 22, 88, 92},{ 17,  0,255},{110,103,214},
    {143,  0,131},{102, 49, 98},{173,  0,  0},{194, 93, 93},{100,102,  0},{ 81, 82, 39},{  0, 71,  2},{ 29, 61, 30},
    { 17, 69, 71},{ 16,  0,235},{100, 93,194},{122,  0,112},{ 92, 44, 88},{153,  0,  0},{163, 78, 78},{ 90, 92,  0},
    { 71, 71, 34},{  0, 61,  2},{ 24, 51, 25},{ 12, 49, 51},{ 14,  0,204},{ 89, 83,173},{112,  0,103},{ 82, 39, 78},
    {143,  0,  0},{143, 69, 69},{ 80, 82,  0},{ 61, 61, 29},{  0, 51,  2},{184,255,186},{122,248,255},{ 12,  0,173},
    { 79, 73,153},{102,  0, 94},{ 71, 34, 68},{133,  0,  0},{122, 59, 59},{ 70, 71,  0},{ 51, 51, 24},{ 61,255, 68},
    {169,235,171},{113,229,235},{ 10,  0,153},{ 63, 59,122},{ 92,  0, 84},{ 61, 29, 59},{122,  0,  0},{112, 54, 54},
    { 60, 61,  0},{254,255,184},{ 54,224, 60},{154,214,156},{103,209,214},{ 10,  0,143},{ 58, 54,112},{ 82,  0, 75},
    { 51, 24, 49},{112,  0,  0},{102, 49, 49},{ 50, 51,  0},{234,235,169},{ 49,204, 54},{140,194,141},{ 93,189,194},
    {  9,  0,133},{ 52, 49,102},{ 71,  0, 65},{245,176,239},{102,  0,  0},{ 92, 44, 44},{242,245, 59},{213,214,154},
    { 44,184, 49},{125,173,126},{ 78,159,163},{  8,  0,122},{ 47, 44, 92},{ 61,  0, 56},{224,162,219},{ 92,  0,  0},
    { 82, 39, 39},{222,224, 54},{193,194,140},{ 39,163, 43},{110,153,112},{ 69,139,143},{  7,  0,112},{ 42, 39, 82},
    { 51,  0, 47},{194,140,189},{ 82,  0,  0},{ 71, 34, 34},{191,194, 47},{173,173,125},{ 34,143, 38},{ 95,133, 97},
    { 59,119,122},{  7,  0,102},{ 37, 34, 71},{245, 59,229},{173,125,169},{ 71,  0,  0},{ 61, 29, 29},{171,173, 42},
    {152,153,110},{ 32,133, 35},{ 81,112, 82},{ 54,109,112},{  6,  0, 92},{ 31, 29, 61},{224, 54,210},{153,110,149},
    { 61,  0,  0},{ 51, 24, 24},{141,143, 34},{122,122, 88},{ 29,122, 32},{ 73,102, 74},{ 49, 99,102},{  5,  0, 82},
    { 26, 24, 51},{204, 49,191},{122, 88,120},{ 51,  0,  0},{245,176,176},{131,133, 32},{112,112, 81},{ 27,112, 30},
    { 66, 92, 67},{ 44, 89, 92},{  5,  0, 71},{188,184,255},{184, 44,172},{102, 73,100},{255, 61, 61},{214,154,154},
    {121,122, 29},{102,102, 73},{ 24,102, 27},{ 59, 82, 60},{ 39, 79, 82},{  4,  0, 61},{173,169,235},{163, 39,153},
    { 92, 66, 90},{235, 56, 56},{184,132,132},{111,112, 27},{ 91, 92, 66},{ 22, 92, 24},{ 51, 71, 52},{ 34, 70, 71},
    {  3,  0, 51},{158,154,214},{143, 34,134},{ 82, 59, 80},{214, 51, 51},{153,110,110},{101,102, 24},{ 81, 82, 59},
    { 20, 82, 22},{ 44, 61, 45},{ 29, 60, 61},{ 71, 59,245},{143,140,194},{122, 29,115},{ 71, 51, 70},{194, 47, 47},
    {122, 88, 88},{ 91, 92, 22},{ 71, 71, 51},{ 17, 71, 19},{ 37, 51, 37},{ 24, 50, 51},{ 62, 51,214},{128,125,173},
    {112, 27,105},{ 61, 44, 60},{173, 42, 42},{102, 73, 73},{ 81, 82, 20},{ 61, 61, 44},{ 15, 61, 16},{  0,242,255},
    {176,241,245},{ 53, 44,184},{113,110,153},{102, 24, 96},{ 51, 37, 50},{153, 37, 37},{ 92, 66, 66},{ 70, 71, 17}
  };

  return lookup[index%256];
}

//##################################################################################################
void validateColor(tp_pipeline::Parameter& param, const TPPixel& color)
{
  if(param.value.index() == 0)
    param.value = color.toString();
  else
    param.value = TPPixel(tpGetVariantValue<std::string>(param.value)).toString();
}

REGISTER_CREATE_STEP_DELEGATES;

//##################################################################################################
int staticInit()
{
  return 0;
}

}
