#include "tp_pipeline_image_utils/step_delegates/FindShapesStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/LineCollectionMember.h"
#include "tp_data_image_utils/members/GridMember.h"

#include "tp_image_utils_functions/FindLines.h"
#include "tp_image_utils_functions/FindPixelGrid.h"

#include "tp_image_utils/Point.h"
#include "tp_image_utils/SaveImages.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

#include "tp_utils/DebugUtils.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
FindShapesStepDelegate::FindShapesStepDelegate():
  AbstractStepDelegate(findShapesSID(), {findAndSegmentSID()})
{

}

//##################################################################################################
void FindShapesStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                         const tp_pipeline::StepInput& input,
                                         tp_data::Collection& output) const
{
  int minPoints          = stepDetails->parameterValue<int>        (       minPointsSID());
  int maxDeviation       = stepDetails->parameterValue<int>        (    maxDeviationSID());
  int maxJointDistance   = stepDetails->parameterValue<int>        (maxJointDistanceSID());
  std::string shapeType  = stepDetails->parameterValue<std::string>(       shapeTypeSID());
  float angleDeviation   = stepDetails->parameterValue<float>      (  angleDeviationSID());
  std::string debugImage = stepDetails->parameterValue<std::string>(  drawDebugImageSID());

  std::string srcName    = stepDetails->parameterValue<std::string>(          sourceSID());

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(srcName, src);
  if(!src)
  {
    output.addError("Failed to find input.");
    return;
  }

  tp_image_utils::LineCollection lines;
  bool linesValid=false;
  tp_image_utils::LineCollection vLines;
  tp_image_utils::LineCollection hLines;
  tp_image_utils::Line distortedGrid;
  tp_image_utils::Grid grid;
  bool gridValid=false;

  if(shapeType == "Lines")
  {
    lines = tp_image_utils_functions::FindLines::findLines(src->data, minPoints, maxDeviation);
    linesValid=true;
  }
  else if(shapeType == "Polylines")
  {
    lines = tp_image_utils_functions::FindLines::findPolylines(src->data, minPoints, maxDeviation, maxJointDistance);
    linesValid=true;
  }
  else if(shapeType == "Polygons")
  {
    lines = tp_image_utils_functions::FindLines::findPolygons(src->data, minPoints, maxDeviation, maxJointDistance);
    linesValid=true;
  }
  else if(shapeType == "Quadrilaterals")
  {
    lines = tp_image_utils_functions::FindLines::findQuadrilaterals(src->data, minPoints, maxDeviation, maxJointDistance);
    linesValid=true;
  }
  else if(shapeType == "Regular finite grid" || shapeType == "Regular infinite grid" || shapeType == "Distorted finite grid")
  {
    lines = tp_image_utils_functions::FindLines::findLines(src->data, minPoints, maxDeviation);
    tp_image_utils_functions::FindPixelGrid::FindRegularGridParams params;
    params.hLines = &hLines;
    params.vLines = &vLines;
    params.angleDeviation = angleDeviation;


    tp_image_utils::GridType gridType = (shapeType == "Regular infinite grid")?tp_image_utils::GridTypeInfinite:tp_image_utils::GridTypeFinite;

    if(shapeType == "Distorted finite grid")
      params.correctedCorners = &distortedGrid;

    grid = tp_image_utils_functions::FindPixelGrid::findRegularGrid(lines, gridType, params);
    gridValid=true;
  }

//  //-- Generate a debug image ----------------------------------------------------------------------
//  if(debugImage == "Yes")
//  {
//    //tp_image_utils::ColorMap img = src->data.toImage();

//    if(!lines.empty())
//    {
//      tp_image_utils::PointStyle style;
//      style.lineColor = TPPixel(255, 0, 0);
//      //img = tp_image_utils_functions::Point::drawPoints(img, lines, style);
//    }

//    if(!vLines.empty())
//    {
//      tp_image_utils::PointStyle style;
//      style.lineColor = TPPixel(0, 255, 0);
//      //img = tp_image_utils_functions::Point::drawPoints(img, vLines, style);
//    }

//    if(!hLines.empty())
//    {
//      tp_image_utils::PointStyle style;
//      style.lineColor = TPPixel(0, 0, 255);
//      //img = tp_image_utils_functions::Point::drawPoints(img, hLines, style);
//    }

//    if(gridValid)
//    {
//      tp_image_utils::GridStyle style;
//      style.lineColor = TPPixel(255, 255, 0);
//      //img = grid.drawGrid(img, style);
//    }

//    if(!distortedGrid.empty())
//    {
//      tp_image_utils::PointStyle style;
//      style.lineColor = TPPixel(255, 0, 255);
//      //img = tp_image_utils::Point::drawPoints(img, distortedGrid, style);
//    }

//    //auto outMember = new ColorMapMember(stepDetails->lookupOutputName("Output data"));
//    //output.addMember(outMember);
//    //outMember->data = img;
//  }

  //-- Output the results --------------------------------------------------------------------------
  {
    if(linesValid)
    {
      auto outMember = new tp_data_image_utils::LineCollectionMember(stepDetails->lookupOutputName("Output shapes"));
      output.addMember(outMember);
      outMember->data = lines;
    }

    if(gridValid)
    {
      auto outMember = new tp_data_image_utils::GridMember(stepDetails->lookupOutputName("Output grid"));
      output.addMember(outMember);
      outMember->data = grid;
    }

    if(!distortedGrid.empty())
    {
      auto outMember = new tp_data_image_utils::LineCollectionMember(stepDetails->lookupOutputName("Distorted grid"));
      output.addMember(outMember);
      outMember->data.push_back(distortedGrid);
    }
  }
}

//##################################################################################################
void FindShapesStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data", "Output shapes", "Output grid", "Distorted grid"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = sourceSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The binary image to search for shapes.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = shapeTypeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The type of shapes to look for.";
    param.setEnum({"Lines",
                   "Polylines",
                   "Polygons",
                   "Quadrilaterals",
                   "Regular finite grid",
                   "Regular infinite grid",
                   "Distorted finite grid"});

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = drawDebugImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "Draw an image showing the intermediate generated lines.";
    param.setEnum({"Yes",
                   "No"});

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = minPointsSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The minimum number of points in a line.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 1000;
    param.validateBounds<int>(30);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = maxDeviationSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum deviation from the line for a point to be considerd part of the line.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 1000;
    param.validateBounds<int>(10);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = maxJointDistanceSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum distance between 2 line ends for them to be considered a possible intersection.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 1000;
    param.validateBounds<int>(100);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = angleDeviationSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = " The maximum difference between the grid orientation and a line for it to be used as part of the grid calculation.";
    param.type = "Float";
    param.min = 0.01f;
    param.max = 30.0f;
    param.validateBounds<float>(2.0f);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
