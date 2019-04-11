#include "tp_pipeline_image_utils/step_delegates/AddBorderStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/AddBorder.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
AddBorderStepDelegate::AddBorderStepDelegate():
  AbstractStepDelegate(addBorderSID(), {processingSID()})
{

}

//##################################################################################################
void AddBorderStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                        const tp_pipeline::StepInput& input,
                                        tp_data::Collection& output) const
{
  TP_UNUSED(stepDetails);

  TPPixel color(stepDetails->parameterValue<std::string>(colorSID()));

  uint8_t value = uint8_t(stepDetails->parameterValue<int>(valueSID()));

  size_t width = stepDetails->parameterValue<size_t>("Width");
  width = tpBound(size_t(0), width, size_t(1000));

  if(input.previousSteps.empty())
  {
    output.addError("No input data found.");
    return;
  }

  for(const auto& member : input.previousSteps.back()->members())
  {

    if(auto byteMapMember = dynamic_cast<tp_data_image_utils::ByteMapMember*>(member); byteMapMember)
    {
      auto newByteMapMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
      output.addMember(newByteMapMember);
      newByteMapMember->data = tp_image_utils_functions::addBorder(byteMapMember->data, width, value);
    }

    else if(auto colorMapMember = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member); colorMapMember)
    {
      auto newColorMapMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output image"));
      output.addMember(newColorMapMember);
      newColorMapMember->data = tp_image_utils_functions::addBorder(colorMapMember->data, width, color);
    }
  }
}

//##################################################################################################
void AddBorderStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = valueSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The value to fill the border with on mono images.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 255;
    param.validateBounds(0);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color to fill the border with on color images.";
    param.type = colorSID();

    validateColor(param, TPPixel("#000000"));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Width";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The width of the border in pixels.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 1000;

    param.validateBounds<int>(10);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
