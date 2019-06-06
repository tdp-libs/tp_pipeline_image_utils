#include "tp_pipeline_image_utils/step_delegates/ToMonoStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils/ToMono.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ToMonoStepDelegate::ToMonoStepDelegate():
  AbstractStepDelegate(toMonoSID(), {conversionSID()})
{

}

//##################################################################################################
void ToMonoStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                     const tp_pipeline::StepInput& input,
                                     tp_data::Collection& output) const
{
  TP_UNUSED(stepDetails);

  int         colorThreshold = stepDetails->parameterValue<int>        (colorThresholdSID());
  int         monoThreshold  = stepDetails->parameterValue<int>        ( monoThresholdSID());
  std::string colorName      = stepDetails->parameterValue<std::string>(    colorImageSID());
  std::string grayName       = stepDetails->parameterValue<std::string>(     grayImageSID());

  colorThreshold = tpBound(1, colorThreshold, 767);
  monoThreshold = tpBound(1, monoThreshold, 254);

  auto processColor = [&](const tp_data_image_utils::ColorMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);
    outMember->data = tp_image_utils::toMono(src->data, colorThreshold);
  };

  auto processGray = [&](const tp_data_image_utils::ByteMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);
    outMember->data = tp_image_utils::toMono(src->data, uint8_t(monoThreshold));
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

  if(!grayName.empty())
  {
    const tp_data_image_utils::ByteMapMember* src{nullptr};
    input.memberCast(grayName, src);

    if(src)
      processGray(src);
    else
      output.addError("Failed to find source gray image.");
  }

  if(colorName.empty() && grayName.empty())
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

      else if(auto gray = dynamic_cast<tp_data_image_utils::ByteMapMember*>(member); gray)
        processGray(gray);
    }
  }
}

//##################################################################################################
void ToMonoStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = monoThresholdSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The threshold between black and white output.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 254;

    if(param.value.index() == 0)
      param.value = 127;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorThresholdSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The threshold between black and white output.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 767;

    if(param.value.index() == 0)
      param.value = 384;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to convert to mono.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to convert to mono.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

//##################################################################################################
tp_pipeline::StepDetails* ToMonoStepDelegate::makeStepDetails()
{
  auto stepDetails = new tp_pipeline::StepDetails(toMonoSID());
  ToMonoStepDelegate().fixupParameters(stepDetails);
  return stepDetails;
}

}
