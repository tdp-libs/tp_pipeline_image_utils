#ifndef tp_pipeline_image_utils_ScaleStepDelegate_h
#define tp_pipeline_image_utils_ScaleStepDelegate_h

#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
class ScaleStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  ScaleStepDelegate();

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;

  //################################################################################################
  enum class SizeCalculation
  {
    UseWidthHeight,
    MaintainAspectMinSize,
    MaintainAspectMaxSize
  };

  //################################################################################################
  static const std::vector<std::string>& sizeCalculationStrings();

  //################################################################################################
  static SizeCalculation sizeCalculationFromString(const std::string& sizeCalculation);

  //################################################################################################
  static std::string sizeCalculationToString(SizeCalculation sizeCalculation);

  //################################################################################################
  //! This generate the step details for a scale step
  static tp_pipeline::StepDetails* makeStepDetails(size_t width, size_t height);

  //################################################################################################
  static tp_pipeline::StepDetails* makeStepDetails(SizeCalculation sizeCalculation, size_t size);

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
