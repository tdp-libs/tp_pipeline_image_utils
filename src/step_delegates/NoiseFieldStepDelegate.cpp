#include "tp_pipeline_image_utils/step_delegates/NoiseFieldStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/NoiseField.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

#define MIN_CELL_SIZE 2
#define MAX_CELL_SIZE 10000

namespace tp_pipeline_image_utils
{

//##################################################################################################
NoiseFieldStepDelegate::NoiseFieldStepDelegate():
  AbstractStepDelegate(noiseFieldSID(), {processingSID()})
{

}

//##################################################################################################
void NoiseFieldStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                         const tp_pipeline::StepInput& input,
                                         tp_data::Collection& output) const
{
  int cellSize = stepDetails->parameterValue<int>("Cell size");
  cellSize = tpBound(MIN_CELL_SIZE, cellSize, MAX_CELL_SIZE);

  if(input.previousSteps.empty())
  {
    output.addError("No input data found.");
    return;
  }

  for(const auto& member : input.previousSteps.back()->members())
  {
    auto byteMapMember = dynamic_cast<tp_data_image_utils::ByteMapMember*>(member);
    if(!byteMapMember)
      continue;

    auto newByteMapMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(newByteMapMember);
    newByteMapMember->data = tp_image_utils_functions::noiseFieldGrid(byteMapMember->data, cellSize);
  }
}

//##################################################################################################
void NoiseFieldStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Cell size";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The size of cell to use for calculating the noise value.";
    param.type = tp_pipeline::intSID();
    param.min = MIN_CELL_SIZE;
    param.max = MAX_CELL_SIZE;

    if(param.value.index() == 0)
      param.value = 10;

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
