#include "tp_pipeline_image_utils/step_delegates/ExtractPolygonsStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_data_math_utils/members/PolygonsMember.h"

#include "tp_image_utils_functions/ExtractPolygons.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

#include "tp_utils/DebugUtils.h"

#include <cmath>

namespace tp_pipeline_image_utils
{

//##################################################################################################
ExtractPolygonsStepDelegate::ExtractPolygonsStepDelegate():
  AbstractStepDelegate(extractPolygonsSID(), {findAndSegmentSID()})
{

}

//##################################################################################################
void ExtractPolygonsStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                              const tp_pipeline::StepInput& input,
                                              tp_data::Collection& output) const
{
  std::string grayName = stepDetails->parameterValue<std::string>(grayImageSID());

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(grayName, src);
  if(src)
  {
    auto outMember = new tp_data_math_utils::PolygonsMember(stepDetails->lookupOutputName("Output polygon"));
    output.addMember(outMember);
    tp_image_utils_functions::ExtractPolygon::simplePolygonExtraction(src->data, outMember->data);
  }
}

//##################################################################################################
void ExtractPolygonsStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output polygon"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    const tp_utils::StringID& name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source gray scale image.";
    param.setNamedData();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
