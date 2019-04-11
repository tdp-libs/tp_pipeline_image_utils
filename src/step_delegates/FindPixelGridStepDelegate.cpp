#include "tp_pipeline_image_utils/step_delegates/FindPixelGridStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/FindPixelGrid.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
FindPixelGridStepDelegate::FindPixelGridStepDelegate():
  AbstractStepDelegate(findPixelGridSID(), {findAndSegmentSID()})
{

}

//##################################################################################################
void FindPixelGridStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                            const tp_pipeline::StepInput& input,
                                            tp_data::Collection& output) const
{
  std::string  srcName = stepDetails->parameterValue<std::string>(gridSourceSID());
  std::string src2Name = stepDetails->parameterValue<std::string>(colorImageSID());

  tp_data_image_utils::ByteMapMember*   src{nullptr};
  tp_data_image_utils::ColorMapMember* src2{nullptr};

  input.memberCast( srcName,  src);
  input.memberCast(src2Name, src2);

  if(src)
  {
    if(src2)
    {
      auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output image"));
      output.addMember(outMember);
      outMember->data = tp_image_utils_functions::FindPixelGrid::findPixelGrid(src->data, src2->data);
    }
    else
    {
      auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
      output.addMember(outMember);
      outMember->data = tp_image_utils_functions::FindPixelGrid::findPixelGrid(src->data);
    }
  }
}

//##################################################################################################
void FindPixelGridStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Grid source";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The byte map to extract the pixel grid from, if this is blank the first byte map will be used.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = colorImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to pixelate, if this is blank the grid source will be used.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
