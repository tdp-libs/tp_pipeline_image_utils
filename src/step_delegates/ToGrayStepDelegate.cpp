#include "tp_pipeline_image_utils/step_delegates/ToGrayStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils/ToGray.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ToGrayStepDelegate::ToGrayStepDelegate():
  AbstractStepDelegate(toGraySID(), {conversionSID()})
{

}

//##################################################################################################
void ToGrayStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                     const tp_pipeline::StepInput& input,
                                     tp_data::Collection& output) const
{
  std::string colorName = stepDetails->parameterValue<std::string>(colorImageSID());  

  auto processColor = [&](const tp_data_image_utils::ColorMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(outMember);
    outMember->data = tp_image_utils::toGray(src->data);
  };

  if(!colorName.empty())
  {
    const tp_data_image_utils::ColorMapMember* src{nullptr};
    input.memberCast(colorName, src);

    if(src)
      processColor(src);
    else
      output.addError("Failed to find source color image.");
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
        processColor(color);
    }
  }
}

//##################################################################################################
void ToGrayStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to convert to mono.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
