#include "tp_pipeline_image_utils/step_delegates/SignedDistanceFieldStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/SignedDistanceField.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
SignedDistanceFieldStepDelegate::SignedDistanceFieldStepDelegate():
  AbstractStepDelegate(signedDistanceFieldSID(), {processingSID()})
{

}

//##################################################################################################
void SignedDistanceFieldStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                                  const tp_pipeline::StepInput& input,
                                                  tp_data::Collection& output) const
{
  int radius = stepDetails->parameterValue<int>(radiusSID());
  int width  = stepDetails->parameterValue<int>( widthSID());
  int height = stepDetails->parameterValue<int>(heightSID());

  radius = tpBound(1, radius,   254);
  width  = tpBound(0, width,  10000);
  height = tpBound(0, height, 10000);

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

    if(width>0 && height>0)
      newByteMapMember->data = tp_image_utils_functions::signedDistanceField(byteMapMember->data, radius, width, height);
    else
      newByteMapMember->data = tp_image_utils_functions::signedDistanceField(byteMapMember->data, radius);
  }
}

//##################################################################################################
void SignedDistanceFieldStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Radius";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The signed distance field radius.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 254;

    if(param.value.index() == 0)
      param.value = 10;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Width";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The destination width.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 10000;

    if(param.value.index() == 0)
      param.value = 0;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Height";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The destination height.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 10000;

    if(param.value.index() == 0)
      param.value = 0;

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
