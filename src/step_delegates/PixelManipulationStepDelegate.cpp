#include "tp_pipeline_image_utils/step_delegates/PixelManipulationStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/PixelManipulation.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{
namespace
{
//##################################################################################################
enum class OutMode_lt
{
  Color,
  Byte
};

//##################################################################################################
OutMode_lt outModeFromString(const std::string& mode)
{
  if(mode=="Color") return OutMode_lt::Color        ;
  return OutMode_lt::Byte;
}
}

//##################################################################################################
PixelManipulationStepDelegate::PixelManipulationStepDelegate():
  AbstractStepDelegate(pixelManipulationSID(), {conversionSID()})
{

}

//##################################################################################################
void PixelManipulationStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                     const tp_pipeline::StepInput& input,
                                     tp_data::Collection& output) const
{
  std::string colorName      = stepDetails->parameterValue<std::string>(    colorImageSID());
  std::string grayName       = stepDetails->parameterValue<std::string>(     grayImageSID());

  OutMode_lt outMode = outModeFromString(stepDetails->parameterValue<std::string>(modeSID()));

  tp_image_utils_functions::PixelManipulation params;
  params.calcRed   = stepDetails->parameterValue<std::string>(calcRedSID  ());
  params.calcGreen = stepDetails->parameterValue<std::string>(calcGreenSID());
  params.calcBlue  = stepDetails->parameterValue<std::string>(calcBlueSID ());
  params.calcAlpha = stepDetails->parameterValue<std::string>(calcAlphaSID());
  params.calcByte  = stepDetails->parameterValue<std::string>(calcByteSID ());

  std::vector<std::string> errors;

  auto process = [&](auto src)
  {
    if(outMode == OutMode_lt::Color)
    {
      auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
      output.addMember(outMember);
      outMember->data = tp_image_utils_functions::pixelManipulationColor(src->data, params, errors);
    }
    else
    {
      auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
      output.addMember(outMember);
      outMember->data = tp_image_utils_functions::pixelManipulationByte(src->data, params, errors);
    }
  };

  if(!colorName.empty())
  {
    const tp_data_image_utils::ColorMapMember* src{nullptr};
    input.memberCast(colorName, src);

    if(src)
      process(src);
    else
      output.addError("Failed to find source color image.");
  }

  if(!grayName.empty())
  {
    const tp_data_image_utils::ByteMapMember* src{nullptr};
    input.memberCast(grayName, src);

    if(src)
      process(src);
    else
      output.addError("Failed to find source gray image.");
  }

  for(const auto& error : errors)
    output.addError(error);
}

//##################################################################################################
void PixelManipulationStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = modeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The type of output to generate.";
    param.setEnum({"Color",
                   "Byte"});
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  OutMode_lt outMode = outModeFromString(stepDetails->parameterValue<std::string>(modeSID()));

  {
    const tp_utils::StringID& name = calcRedSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The expression used to calculate the red component.";
    param.type = tp_pipeline::stringSID();
    param.value = tpGetVariantValue<std::string>(param.value, "red");
    param.enabled = outMode == OutMode_lt::Color;
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = calcGreenSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The expression used to calculate the green component.";
    param.type = tp_pipeline::stringSID();
    param.value = tpGetVariantValue<std::string>(param.value, "green");
    param.enabled = outMode == OutMode_lt::Color;
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = calcBlueSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The expression used to calculate the blue component.";
    param.type = tp_pipeline::stringSID();
    param.value = tpGetVariantValue<std::string>(param.value, "blue");
    param.enabled = outMode == OutMode_lt::Color;
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = calcAlphaSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The expression used to calculate the alpha component.";
    param.type = tp_pipeline::stringSID();
    param.value = tpGetVariantValue<std::string>(param.value, "alpha");
    param.enabled = outMode == OutMode_lt::Color;
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = calcByteSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The expression used to calculate the byte component.";
    param.type = tp_pipeline::stringSID();
    param.value = tpGetVariantValue<std::string>(param.value, "byte");
    param.enabled = outMode == OutMode_lt::Byte;
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

}
