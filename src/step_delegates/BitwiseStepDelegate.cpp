#include "tp_pipeline_image_utils/step_delegates/BitwiseStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/Bitwise.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

namespace tp_pipeline_image_utils
{
//##################################################################################################
BitwiseStepDelegate::BitwiseStepDelegate():
  AbstractStepDelegate(bitwiseSID(), {processingSID()})
{

}

//##################################################################################################
void BitwiseStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                      const tp_pipeline::StepInput& input,
                                      tp_data::Collection& output) const
{
  auto operation = tp_image_utils_functions::logicOpFromString(stepDetails->parameterValue<std::string>("Logical operation"));

  std::string pName = stepDetails->parameterValue<std::string>("P");
  std::string qName = stepDetails->parameterValue<std::string>("Q");

  tp_data_image_utils::ByteMapMember* p{nullptr};
  tp_data_image_utils::ByteMapMember* q{nullptr};

  input.memberCast(pName, p);
  input.memberCast(qName, q);

  if(!p)
    p=q;

  if(!q)
    q=p;

  if(p&&q)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output image"));
    output.addMember(outMember);
    outMember->data = tp_image_utils_functions::bitwise(p->data, q->data, operation);
  }
  else
  {
    output.addError("Failed to find 2 inputs.");
  }
}

//##################################################################################################
void BitwiseStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = "Logical operation";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The bitwise operation to perform.";
    param.setEnum(tp_image_utils_functions::logicalOps());

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "P";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The P input image.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = "Q";
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The Q input image.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
