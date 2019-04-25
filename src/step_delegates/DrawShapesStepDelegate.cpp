#include "tp_pipeline_image_utils/step_delegates/DrawShapesStepDelegate.h"
#include "tp_data_image_utils/members/GridMember.h"
#include "tp_data_image_utils/members/LineCollectionMember.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/DrawShapes.h"

#include "tp_image_utils/Point.h"
#include "tp_image_utils/SaveImages.h"

#include "tp_pipeline/StepInput.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_data/Collection.h"

#include "tp_utils/DebugUtils.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
DrawShapesStepDelegate::DrawShapesStepDelegate():
  AbstractStepDelegate(drawShapesSID(), {drawingSID()})
{

}

//##################################################################################################
void DrawShapesStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                         const tp_pipeline::StepInput& input,
                                         tp_data::Collection& output) const
{
  TPPixel color(stepDetails->parameterValue<std::string>(colorSID()));

  std::string imageName = stepDetails->parameterValue<std::string>(colorImageSID());
  std::string linesName = stepDetails->parameterValue<std::string>(     linesSID());
  std::string  gridName = stepDetails->parameterValue<std::string>(      gridSID());

  const tp_data_image_utils::ColorMapMember* image{nullptr};
  input.memberCast(imageName, image);

  if(!image)
    return;

  auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
  output.addMember(outMember);
  outMember->data = image->data;

  {
    const tp_data_image_utils::LineCollectionMember* lines{nullptr};
    input.memberCast(linesName, lines);
    if(lines && !lines->data.empty())
    {
      tp_image_utils::PointStyle style;
      style.lineColor = color;
      outMember->data = tp_image_utils_functions::drawPoints(outMember->data, lines->data, style);
    }
  }

  {
    const tp_data_image_utils::GridMember* grid{nullptr};
    input.memberCast(gridName, grid);
    if(grid)
    {
      tp_image_utils::GridStyle style;
      style.lineColor = color;
      outMember->data = tp_image_utils_functions::drawGrid(grid->data, outMember->data, style);
    }
  }
}

//##################################################################################################
void DrawShapesStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color image to draw over.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = linesSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "A named set of lines to draw.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = gridSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "A named grid to draw.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color to draw the lines with.";
    param.type = colorSID();

    validateColor(param, TPPixel("#00FF00"));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
