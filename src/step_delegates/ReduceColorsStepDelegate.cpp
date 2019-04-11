#include "tp_pipeline_image_utils/step_delegates/ReduceColorsStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/ReduceColors.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ReduceColorsStepDelegate::ReduceColorsStepDelegate():
  AbstractStepDelegate(reduceColorsSID(), {processingSID()})
{

}

//##################################################################################################
void ReduceColorsStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                           const tp_pipeline::StepInput& input,
                                           tp_data::Collection& output) const
{
  std::shared_ptr<tp_data::Collection> results;

  int paletteSize = stepDetails->parameterValue<int>("Palette size");

  if(input.previousSteps.empty())
  {
    output.addError("No input data found.");
    return;
  }

  for(const auto& member : input.previousSteps.back()->members())
  {
    auto byteMapMember = dynamic_cast<tp_data_image_utils::ColorMapMember*>(member);
    if(!byteMapMember)
      continue;

    auto newByteMapMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(newByteMapMember);
    newByteMapMember->data = tp_image_utils_functions::reduceColors(byteMapMember->data, paletteSize);
  }
}

//##################################################################################################
void ReduceColorsStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Palette size";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum number of colors in the palette.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 1000;

    if(param.value.index() == 0)
      param.value = 8;
    else
      param.value = tpBound(tpGetVariantValue<int>(param.min),
                            tpGetVariantValue<int>(param.value),
                            tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
