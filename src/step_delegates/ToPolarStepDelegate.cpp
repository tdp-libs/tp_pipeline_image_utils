#include "tp_pipeline_image_utils/step_delegates/ToPolarStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/ToPolar.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
ToPolarStepDelegate::ToPolarStepDelegate():
  AbstractStepDelegate(toPolarSID(), {conversionSID()})
{

}

//##################################################################################################
void ToPolarStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                      const tp_pipeline::StepInput& input,
                                      tp_data::Collection& output) const
{
  size_t w = stepDetails->parameterValue<size_t>( widthSID());
  size_t h = stepDetails->parameterValue<size_t>(heightSID());

  std::string grayName  = stepDetails->parameterValue<std::string>(grayImageSID());

  auto processGray = [&](const tp_data_image_utils::ByteMapMember* src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(outMember);
    outMember->data = tp_image_utils_functions::toPolar(src->data, w, h);
  };

  if(!grayName.empty())
  {
    const tp_data_image_utils::ByteMapMember* src{nullptr};
    input.memberCast(grayName, src);

    if(src)
      processGray(src);
    else
      output.addError("Failed to find source gray image.");
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
      if(auto gray = dynamic_cast<tp_data_image_utils::ByteMapMember*>(member); gray)
        processGray(gray);
    }
  }
}

//##################################################################################################
void ToPolarStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = widthSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The resolution from the center.";
    param.type = tp_pipeline::intSID();
    param.min = 2;
    param.max = 1024;
    param.validateBounds<int>(64);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = heightSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The number of evenly spaced angles.";
    param.type = tp_pipeline::intSID();
    param.min = 2;
    param.max = 1024;
    param.validateBounds<int>(256);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to convert to polar coords.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
