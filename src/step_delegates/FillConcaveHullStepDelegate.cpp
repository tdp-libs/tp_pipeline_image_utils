#include "tp_pipeline_image_utils/step_delegates/FillConcaveHullStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/FillConcaveHull.h"

#include "tp_pipeline/StepInput.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
FillConcaveHullStepDelegate::FillConcaveHullStepDelegate():
  AbstractStepDelegate(fillConcaveHullSID(), {processingSID()})
{

}

//##################################################################################################
void FillConcaveHullStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                              const tp_pipeline::StepInput& input,
                                              tp_data::Collection& output) const
{
  std::string monoName  = stepDetails->parameterValue<std::string>(monoImageSID());

  tp_image_utils_functions::FillConcaveHullParameters params;

  params.solid = uint8_t(stepDetails->parameterValue<int>(solidSID()));

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(monoName, src);
  if(src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(outMember);
    outMember->data = tp_image_utils_functions::fillConcaveHull(src->data, params);
  }
}

//##################################################################################################
void FillConcaveHullStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    const tp_utils::StringID& name = monoImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source mono image.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  auto addIntParam = [&](const tp_utils::StringID& name, int min, int max, int init, const char* description)
  {
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = description;
    param.type = tp_pipeline::intSID();
    param.min = min;
    param.max = max;

    if(param.value.index() == 0)
      param.value = init;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  };

  addIntParam(          solidSID(), 0,  255,   0, "The value of the solid areas.");

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
