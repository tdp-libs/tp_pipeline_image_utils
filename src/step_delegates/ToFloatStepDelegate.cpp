#include "tp_pipeline_image_utils/step_delegates/ToFloatStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_data_math_utils/members/FloatsMember.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

#include "tp_image_utils_functions/ToFloat.h"

#warning remove
#include "tp_image_utils/SaveImages.h"
#include "tp_utils/DebugUtils.h"

namespace tp_pipeline_image_utils
{

namespace
{
//##################################################################################################
void _fixupParameters(tp_pipeline::StepDetails* stepDetails)
{
  stepDetails->setOutputNames({"Output data"});

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

  {
    tp_utils::StringID name = channelModeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "Select how channels are stored in memory.";
    param.setEnum(tp_image_utils_functions::channelModes());

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = channelOrderSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "Select how channels are ordered in memory.";
    param.setEnum(tp_image_utils_functions::channelOrders());

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}
}

//##################################################################################################
ToFloatStepDelegate::ToFloatStepDelegate():
  AbstractStepDelegate(toFloatSID(), {conversionSID()})
{

}

//##################################################################################################
void ToFloatStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                      const tp_pipeline::StepInput& input,
                                      tp_data::Collection& output) const
{
  std::string colorName = stepDetails->parameterValue<std::string>(colorImageSID());

  auto channelMode  =  tp_image_utils_functions::channelModeFromString(stepDetails->parameterValue<std::string>( channelModeSID()));
  auto channelOrder = tp_image_utils_functions::channelOrderFromString(stepDetails->parameterValue<std::string>(channelOrderSID()));

  auto processColor = [&](const tp_data_image_utils::ColorMapMember* src)
  {
    auto outMember = new tp_data_math_utils::FloatsMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);


    tp_image_utils_functions::toFloat(src->data,
                                      tp_image_utils_functions::ChannelMode::Separate,
                                      tp_image_utils_functions::ChannelOrder::RGB,
                                      outMember->data);

    //tp_image_utils_functions::toFloat(src->data, channelMode, channelOrder, outMember->data);
  };

  if(!colorName.empty())
  {
    const tp_data_image_utils::ColorMapMember* src{nullptr};
    input.memberCast(colorName, src);

    if(src)
    {
      tp_image_utils::saveImage("/home/tom/Desktop/src.png", src->data);
      processColor(src);
    }
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
      {
        tp_image_utils::saveImage("/home/tom/Desktop/out.png", color->data);
        processColor(color);
      }
    }
  }
}

//##################################################################################################
void ToFloatStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  _fixupParameters(stepDetails);
}

//################################################################################################
tp_pipeline::StepDetails* ToFloatStepDelegate::makeStepDetails(const std::string& inName,
                                                               const std::string& outName,
                                                               tp_image_utils_functions::ChannelMode channelMode,
                                                               tp_image_utils_functions::ChannelOrder channelOrder)
{
  auto stepDetails = new tp_pipeline::StepDetails(toFloatSID());
  _fixupParameters(stepDetails);
  stepDetails->setParameterValue(colorImageSID(), inName);
  stepDetails->setOutputMapping({{"Output data", outName}});
  stepDetails->setParameterValue(channelModeSID(), tp_image_utils_functions::channelModeToString(channelMode));
  stepDetails->setParameterValue(channelOrderSID(), tp_image_utils_functions::channelOrderToString(channelOrder));
  return stepDetails;
}

}
