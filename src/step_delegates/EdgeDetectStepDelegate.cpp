#include "tp_pipeline_image_utils/step_delegates/EdgeDetectStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/EdgeDetect.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

namespace
{
//##################################################################################################
enum class Mode_lt
{
  Edge,
  Corner
};

//##################################################################################################
Mode_lt modeFromString(const std::string& mode)
{
  if(mode=="Edge detect")   return Mode_lt::Edge  ;
  if(mode=="Corner detect") return Mode_lt::Corner;

  return Mode_lt::Edge;
}
}

//##################################################################################################
EdgeDetectStepDelegate::EdgeDetectStepDelegate():
  AbstractStepDelegate(edgeDetectSID(), {processingSID()})
{

}

//##################################################################################################
void EdgeDetectStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                         const tp_pipeline::StepInput& input,
                                         tp_data::Collection& output) const
{
  uint8_t colorThreshold = uint8_t(stepDetails->parameterValue<int>(colorThresholdSID()));
  uint8_t grayThreshold  = uint8_t(stepDetails->parameterValue<int>( grayThresholdSID()));

  std::string colorName = stepDetails->parameterValue<std::string>(colorImageSID());
  std::string grayName  = stepDetails->parameterValue<std::string>(grayImageSID());

  Mode_lt mode = modeFromString(stepDetails->parameterValue<std::string>(modeSID()));

  auto processColor = [&](const tp_data_image_utils::ColorMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);

    if(mode == Mode_lt::Edge)
      outMember->data = tp_image_utils_functions::edgeDetect(src->data, colorThreshold);
    else if(mode == Mode_lt::Corner)
      outMember->data = tp_image_utils_functions::edgeDetectCorner(tp_image_utils::ByteMap(src->data), colorThreshold);
  };

  auto processGray = [&](const tp_data_image_utils::ByteMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);

    if(mode == Mode_lt::Edge)
      outMember->data = tp_image_utils_functions::edgeDetect(src->data, grayThreshold);
    else if(mode == Mode_lt::Corner)
      outMember->data = tp_image_utils_functions::edgeDetectCorner(src->data, grayThreshold);
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
void EdgeDetectStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = modeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The type of operation to perform.";
    param.setEnum({"Edge detect",
                   "Corner detect"});

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Color threshold";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The threshold to use for color images.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 440;
    param.validateBounds<int>(15);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Gray threshold";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The threshold to use for gray images.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 255;
    param.validateBounds<int>(10);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to edge detect.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to edge detect.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
