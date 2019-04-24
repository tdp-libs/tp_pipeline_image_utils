#ifndef tp_pipeline_image_utils_ToFloatStepDelegate_h
#define tp_pipeline_image_utils_ToFloatStepDelegate_h

#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline/AbstractStepDelegate.h"

#include "tp_image_utils_functions/ToFloat.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
class ToFloatStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  ToFloatStepDelegate();

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;

  //################################################################################################
  static tp_pipeline::StepDetails* makeStepDetails(const std::string& inName,
                                                   const std::string& outName,
                                                   tp_image_utils_functions::ChannelMode channelMode,
                                                   tp_image_utils_functions::ChannelOrder channelOrder);
};

}

#endif
