#include "tp_pipeline_image_utils/step_delegates/FinalizeStepDelegate.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"
#include "tp_data/CollectionFactory.h"

#include "tp_utils/DebugUtils.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
FinalizeStepDelegate::FinalizeStepDelegate(const tp_data::CollectionFactory* collectionFactory):
  AbstractStepDelegate(finalizeSID(), {loadAndSaveSID()}),
  m_collectionFactory(collectionFactory)
{
}

//##################################################################################################
void FinalizeStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                       const tp_pipeline::StepInput& input,
                                       tp_data::Collection& output) const
{
  for(const auto& pair : stepDetails->outputMapping())
  {
    auto member = input.member(pair.second);
    if(member)
    {
      std::string error;
      auto newMember = m_collectionFactory->clone(error, *member);

      if(newMember)
        output.addMember(newMember);

      if(!error.empty())
        output.addError(error);
    }
  }
}

//##################################################################################################
void FinalizeStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Map"});
}

}
