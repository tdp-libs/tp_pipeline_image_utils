#include "tp_pipeline_image_utils/step_delegates/SlotFillStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/SlotFill.h"

#include "tp_pipeline/StepInput.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
SlotFillStepDelegate::SlotFillStepDelegate():
  AbstractStepDelegate(slotFillSID(), {processingSID()})
{

}

//##################################################################################################
void SlotFillStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                       const tp_pipeline::StepInput& input,
                                       tp_data::Collection& output) const
{
  std::string monoName  = stepDetails->parameterValue<std::string>(monoImageSID());

  tp_image_utils_functions::SlotFillParameters params;

  params.solid = uint8_t(stepDetails->parameterValue<int>(solidSID()));
  params.slot  = uint8_t(stepDetails->parameterValue<int>( slotSID()));

  params.endMinEach     = tpBound(size_t(1), stepDetails->parameterValue<size_t>(     endMinEachSID()), size_t(2048));
  params.endMinEither   = tpBound(size_t(1), stepDetails->parameterValue<size_t>(   endMinEitherSID()), size_t(2048));
  params.endMinSum      = tpBound(size_t(1), stepDetails->parameterValue<size_t>(      endMinSumSID()), size_t(2048));
  params.borderMinEach  = tpBound(size_t(0), stepDetails->parameterValue<size_t>(  borderMinEachSID()), size_t(2048));
  params.borderMinEither= tpBound(size_t(0), stepDetails->parameterValue<size_t>(borderMinEitherSID()), size_t(2048));
  params.borderMinSum   = tpBound(size_t(0), stepDetails->parameterValue<size_t>(   borderMinSumSID()), size_t(2048));

  params.startAngle = tpBound(size_t(0), stepDetails->parameterValue<size_t>(startAngleSID()), size_t(360));
  params.maxAngle   = tpBound(size_t(1), stepDetails->parameterValue<size_t>(  maxAngleSID()), size_t( 90));
  params.stepAngle  = tpBound(size_t(1), stepDetails->parameterValue<size_t>( stepAngleSID()), size_t( 90));

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(monoName, src);
  if(src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);
    outMember->data = tp_image_utils_functions::slotFill(src->data, params);
  }
}

//##################################################################################################
void SlotFillStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

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

  addIntParam(          solidSID(), 0,  255,   0, "The value of the solid areas."                               );
  addIntParam(           slotSID(), 0,  255, 255, "The value of the slot areas."                                );

  addIntParam(     endMinEachSID(), 1, 2048,   1, "Both ends should be at least this long."                     );
  addIntParam(   endMinEitherSID(), 1, 2048,   5, "One or both ends should be at least this long."              );
  addIntParam(      endMinSumSID(), 1, 2048,   6, "The sum of both ends should be at lest this lone."           );
  addIntParam(  borderMinEachSID(), 0, 2048,   1, "Both ends should have a border at least this long."          );
  addIntParam(borderMinEitherSID(), 0, 2048,   1, "One or both ends should have a border at least this long."   );
  addIntParam(   borderMinSumSID(), 0, 2048,   2, "The sum of both ends should have a border at lest this long.");

  addIntParam(     startAngleSID(), 0,  360,   2, "The initial angle to rotate the image by."                   );
  addIntParam(       maxAngleSID(), 1,   90,  90, "The maximum angle to rotate by."                             );
  addIntParam(      stepAngleSID(), 1,   90,  45, "The step to increase the angle by."                          );

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
