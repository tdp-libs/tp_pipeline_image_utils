#include "tp_pipeline_image_utils/step_delegates/NormalizeBrightnessStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/NormalizeBrightness.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
NormalizeBrightnessStepDelegate::NormalizeBrightnessStepDelegate():
  AbstractStepDelegate(normalizeBrightnessSID(), {processingSID()})
{

}

//##################################################################################################
void NormalizeBrightnessStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                                  const tp_pipeline::StepInput& input,
                                                  tp_data::Collection& output) const
{
  int paletteSize        = stepDetails->parameterValue<int>        ("Radius");
  float exaggeration     = stepDetails->parameterValue<float>      ("Exaggeration");
  std::string mode_      = stepDetails->parameterValue<std::string>("Mode");
  std::string shiftMode_ = stepDetails->parameterValue<std::string>("Shift mode");
  int shiftValue         = stepDetails->parameterValue<int>        ("Shift value");

  if(input.previousSteps.empty())
  {
    output.addError("No input data found.");
    return;
  }

  if(mode_ == "Shift brightness")
  {
    tp_image_utils_functions::ShiftBrightnessMode mode = tp_image_utils_functions::shiftBrightnessModeFromString(shiftMode_);

    for(const auto& member : input.previousSteps.back()->members())
    {
      auto colorMapMember = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member);
      if(!colorMapMember)
        continue;

      auto newColorMapMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
      output.addMember(newColorMapMember);
      newColorMapMember->data = colorMapMember->data;
      tp_image_utils_functions::shiftBrightness(newColorMapMember->data, mode, uint8_t(shiftValue));
    }
  }
  else
  {
    tp_image_utils_functions::NormalizationMode mode = tp_image_utils_functions::NormalizationMode::Normalize;

    if(mode_=="Exaggerate")
      mode = tp_image_utils_functions::NormalizationMode::Exaggerate;

    for(const auto& member : input.previousSteps.back()->members())
    {
      auto colorMapMember = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member);
      if(!colorMapMember)
        continue;

      auto newColorMapMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
      output.addMember(newColorMapMember);
      newColorMapMember->data = colorMapMember->data;
      tp_image_utils_functions::normalizeBrightness(newColorMapMember->data, paletteSize, mode, exaggeration);
    }
  }
}

//##################################################################################################
void NormalizeBrightnessStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Radius";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The radius to sample for the average brightness.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 1000;

    param.validateBounds(8);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }


  {
    tp_utils::StringID name = "Exaggeration";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The amount to exagerate the difference by.";
    param.type = "Float";
    param.min = 0.01f;
    param.max = 100.0f;
    param.validateBounds<float>(3.0f);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Mode";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "How to calculate the output value.";
    param.setEnum({"Normalize", "Exaggerate", "Shift brightness"});
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Shift mode";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "If we are shifting this mode controlls how this is performed.";
    param.setEnum({"By value", "By mean", "By mode", "By median", "By soft mode"});
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Shift value";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The amount to shift by if shift mode is by value.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 255;

    param.validateBounds(0);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
