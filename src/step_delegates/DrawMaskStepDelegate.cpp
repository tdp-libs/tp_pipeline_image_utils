#include "tp_pipeline_image_utils/step_delegates/DrawMaskStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils_functions/DrawMask.h"

#include "tp_pipeline/StepInput.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
DrawMaskStepDelegate::DrawMaskStepDelegate():
  AbstractStepDelegate(drawMaskSID(), {drawingSID()})
{

}

//##################################################################################################
void DrawMaskStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                       const tp_pipeline::StepInput& input,
                                       tp_data::Collection& output) const
{
  std::string imageName  = stepDetails->parameterValue<std::string>(colorImageSID());
  std::string maskName  = stepDetails->parameterValue<std::string>(maskSID());

  TPPixel color(stepDetails->parameterValue<std::string>(colorSID()));
  uint8_t value = uint8_t(stepDetails->parameterValue<int>(valueSID()));

  const tp_data_image_utils::ColorMapMember* image{nullptr};
  const tp_data_image_utils::ByteMapMember*   mask{nullptr};

  input.memberCast(imageName, image);
  input.memberCast( maskName,  mask);
  if(image && mask)
  {
    auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);
    outMember->data = image->data;
    tp_image_utils_functions::drawMask(outMember->data, color, mask->data, value);
  }
}

//##################################################################################################
void DrawMaskStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    const tp_utils::StringID& name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color image to draw over.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = maskSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The mask to draw onto the image.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color to draw the mask with.";
    param.type = colorSID();

    validateColor(param, TPPixel("#000000"));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = valueSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The value in the mask that will be drawn.";
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
