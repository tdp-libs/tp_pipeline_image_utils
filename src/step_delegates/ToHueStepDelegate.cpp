#include "tp_pipeline_image_utils/step_delegates/ToHueStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/ToHue.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ToHueStepDelegate::ToHueStepDelegate():
  AbstractStepDelegate(toHueSID(), {conversionSID()})
{

}

//##################################################################################################
void ToHueStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                    const tp_pipeline::StepInput& input,
                                    tp_data::Collection& output) const
{
  std::string outputMode = stepDetails->parameterValue<std::string>("Output mode");

  if(outputMode=="Color")
  {
    if(input.previousSteps.empty())
    {
      output.addError("No input data found.");
      return;
    }

    for(const auto& member : input.previousSteps.back()->members())
    {
      if(auto color = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member); color)
      {
        auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
        output.addMember(outMember);
        outMember->data = tp_image_utils_functions::toHue(color->data);
      }

      else if(auto gray = dynamic_cast<tp_data_image_utils::ByteMapMember*>(member); gray)
      {
        auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
        output.addMember(outMember);
        outMember->data = tp_image_utils_functions::toHue(gray->data);
      }
    }
  }
  else
  {
    if(input.previousSteps.empty())
    {
      output.addError("No input data found.");
      return;
    }

    for(const auto& member : input.previousSteps.back()->members())
    {
      if(auto color = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member); color)
      {
        auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
        output.addMember(outMember);
        outMember->data = tp_image_utils_functions::toHueGray(color->data);
      }
    }
  }
}

//##################################################################################################
void ToHueStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Output mode";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color space to output the results in.";
    param.setEnum({"Color", "Gray"});
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
