#include "tp_pipeline_image_utils/step_delegates/ColorizeStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ColorizeStepDelegate::ColorizeStepDelegate():
  AbstractStepDelegate(colorizeSID(), {drawingSID()})
{

}

//##################################################################################################
void ColorizeStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                       const tp_pipeline::StepInput& input,
                                       tp_data::Collection& output) const
{
  std::string grayName  = stepDetails->parameterValue<std::string>(grayImageSID());

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(grayName, src);
  if(!src)
    return;

  size_t w = src->data.width();
  size_t h = src->data.height();

  auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
  output.addMember(outMember);
  outMember->data.setSize(w, h);

  const uint8_t* s = src->data.constData();
  const uint8_t* sMax = s + src->data.size();
  TPPixel* dst = outMember->data.data();

  for(; s<sMax; s++, dst++)
    (*dst) = makeColor(*s);
}

//##################################################################################################
void ColorizeStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    const tp_utils::StringID& name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source gray image.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
