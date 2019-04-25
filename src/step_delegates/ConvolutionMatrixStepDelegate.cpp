#include "tp_pipeline_image_utils/step_delegates/ConvolutionMatrixStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/ConvolutionMatrix.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ConvolutionMatrixStepDelegate::ConvolutionMatrixStepDelegate():
  AbstractStepDelegate(tp_pipeline::convolutionMatrixSID(), {processingSID()})
{

}

//##################################################################################################
void ConvolutionMatrixStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                                const tp_pipeline::StepInput& input,
                                                tp_data::Collection& output) const
{
  tp_image_utils_functions::ConvolutionMatrix matrix(stepDetails->parameterValue<std::string>("Matrix"));

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

    auto newByteMapMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(newByteMapMember);
    newByteMapMember->data = matrix.convolve(byteMapMember->data);
  }
}

//##################################################################################################
void ConvolutionMatrixStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Matrix";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The matrix parameters to use for the convolution.";
    param.type = tp_pipeline::convolutionMatrixSID();

    param.value = tp_image_utils_functions::ConvolutionMatrix(tpGetVariantValue<std::string>(param.value)).toString();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
