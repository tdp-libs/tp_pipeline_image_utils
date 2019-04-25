#include "tp_pipeline_image_utils/step_delegates/DeNoiseStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/DeNoise.h"

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
  RemoveWhite,
  RemoveBlack,
  RemoveBoth,
  BlackStripeRemoval,
  WhiteStripeRemoval,
  BlackBlobRemoval,
  WhiteBlobRemoval,
  BlackKnobletRemoval,
  WhiteKnobletRemoval
};

//##################################################################################################
//std::string modeToString(Mode_lt mode)
//{
//  switch(mode)
//  {
//  case Mode_lt::RemoveWhite         : return "Remove white"         ;
//  case Mode_lt::RemoveBlack         : return "Remove black"         ;
//  case Mode_lt::RemoveBoth          : return "Remove both"          ;
//  case Mode_lt::BlackStripeRemoval  : return "Black stripe removal" ;
//  case Mode_lt::WhiteStripeRemoval  : return "White stripe removal" ;
//  case Mode_lt::BlackBlobRemoval    : return "Black blob removal"   ;
//  case Mode_lt::WhiteBlobRemoval    : return "White blob removal"   ;
//  case Mode_lt::BlackKnobletRemoval : return "Black knoblet removal";
//  case Mode_lt::WhiteKnobletRemoval : return "White knoblet removal";
//  }
//
//  return "Remove white";
//}

//##################################################################################################
Mode_lt modeFromString(const std::string& mode)
{
  if(mode=="Remove white")          return Mode_lt::RemoveWhite        ;
  if(mode=="Remove black")          return Mode_lt::RemoveBlack        ;
  if(mode=="Remove both")           return Mode_lt::RemoveBoth         ;
  if(mode=="Black stripe removal")  return Mode_lt::BlackStripeRemoval ;
  if(mode=="White stripe removal")  return Mode_lt::WhiteStripeRemoval ;
  if(mode=="Black blob removal")    return Mode_lt::BlackBlobRemoval   ;
  if(mode=="White blob removal")    return Mode_lt::WhiteBlobRemoval   ;
  if(mode=="Black knoblet removal") return Mode_lt::BlackKnobletRemoval;
  if(mode=="White knoblet removal") return Mode_lt::WhiteKnobletRemoval;

  return Mode_lt::RemoveWhite;
}
}

//##################################################################################################
DeNoiseStepDelegate::DeNoiseStepDelegate():
  AbstractStepDelegate(deNoiseSID(), {processingSID()})
{

}

//##################################################################################################
void DeNoiseStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                      const tp_pipeline::StepInput& input,
                                      tp_data::Collection& output) const
{
  std::string grayName  = stepDetails->parameterValue<std::string>(grayImageSID());

  int noiseThreshold = stepDetails->parameterValue<int>(noiseThresholdSID());

  float minAspectRatio = stepDetails->parameterValue<float>(minAspectRatioSID());
  float maxAspectRatio = stepDetails->parameterValue<float>(maxAspectRatioSID());
  float minDensity     = stepDetails->parameterValue<float>(minDensitySID());
  float maxDensity     = stepDetails->parameterValue<float>(maxDensitySID());
  int minSize          = stepDetails->parameterValue<int>(minSizeSID());
  int maxSize          = stepDetails->parameterValue<int>(maxSizeSID());
  int knobletWidth     = stepDetails->parameterValue<int>(knobletWidthSID());

  Mode_lt mode = modeFromString(stepDetails->parameterValue<std::string>(modeSID()));

  bool addCorners = (stepDetails->parameterValue<std::string>(cornerModeSID()) == "Include");

  auto processGray = [&](const tp_image_utils::ByteMap& src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);

    if(mode==Mode_lt::BlackStripeRemoval)
      outMember->data = tp_image_utils_functions::deNoiseStripes(src, noiseThreshold, 0, 255);
    else if(mode==Mode_lt::WhiteStripeRemoval)
      outMember->data = tp_image_utils_functions::deNoiseStripes(src, noiseThreshold, 255, 0);
    else if(mode==Mode_lt::BlackBlobRemoval)
      outMember->data = tp_image_utils_functions::deNoiseBlobs(src,
                                                               minAspectRatio,
                                                               maxAspectRatio,
                                                               minDensity,
                                                               maxDensity,
                                                               minSize,
                                                               maxSize,
                                                               addCorners,
                                                               0,
                                                               255);
    else if(mode==Mode_lt::WhiteBlobRemoval)
      outMember->data = tp_image_utils_functions::deNoiseBlobs(src,
                                                               minAspectRatio,
                                                               maxAspectRatio,
                                                               minDensity,
                                                               maxDensity,
                                                               minSize,
                                                               maxSize,
                                                               addCorners,
                                                               255,
                                                               0);
    else if(mode==Mode_lt::BlackKnobletRemoval)
      outMember->data = tp_image_utils_functions::deNoiseKnoblets(src, knobletWidth, 0, 255);
    else if(mode==Mode_lt::WhiteKnobletRemoval)
      outMember->data = tp_image_utils_functions::deNoiseKnoblets(src, knobletWidth, 255, 0);
    else
    {
      if(mode==Mode_lt::RemoveBlack || mode==Mode_lt::RemoveBoth)
        outMember->data = tp_image_utils_functions::deNoise(src,
                                                            noiseThreshold,
                                                            addCorners,
                                                            0,
                                                            255);

      if(mode==Mode_lt::RemoveWhite || mode==Mode_lt::RemoveBoth)
        outMember->data = tp_image_utils_functions::deNoise(src,
                                                            noiseThreshold,
                                                            addCorners,
                                                            255,
                                                            0);
    }
  };

  if(!grayName.empty())
  {
    const tp_data_image_utils::ByteMapMember* src{nullptr};
    input.memberCast(grayName, src);

    if(src)
      processGray(src->data);
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

    for(const auto member : input.previousSteps.back()->members())
    {
      auto src = dynamic_cast<const tp_data_image_utils::ByteMapMember*>(member);
      if(src)
        processGray(src->data);
    }
  }
}

//##################################################################################################
void DeNoiseStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  {
    tp_utils::StringID name = grayImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source image to de noise.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = modeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The color space to output the results in.";
    param.setEnum({"Remove white",
                   "Remove black",
                   "Remove both",
                   "Black stripe removal",
                   "White stripe removal",
                   "Black blob removal",
                   "White blob removal",
                   "Black knoblet removal",
                   "White knoblet removal"});

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  Mode_lt mode = modeFromString(stepDetails->parameterValue<std::string>(modeSID()));

  {
    tp_utils::StringID name = cornerModeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "Should we include corners in the segmentation.";
    param.setEnum({"Include", "Exclude"});

    param.enabled = (mode==Mode_lt::BlackBlobRemoval ||
                     mode==Mode_lt::WhiteBlobRemoval ||
                     mode==Mode_lt::RemoveBlack      ||
                     mode==Mode_lt::RemoveWhite      ||
                     mode==Mode_lt::RemoveBoth);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = noiseThresholdSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum area of a region for it to be considered noise.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 100000;
    param.validateBounds<int>(10);

    param.enabled = (mode==Mode_lt::BlackStripeRemoval ||
                     mode==Mode_lt::WhiteStripeRemoval ||
                     mode==Mode_lt::RemoveBlack        ||
                     mode==Mode_lt::RemoveWhite        ||
                     mode==Mode_lt::RemoveBoth);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = minAspectRatioSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The minimum aspect ratio for blob removal.";
    param.type = tp_pipeline::floatSID();
    param.min = 0.0f;
    param.max = 1.0f;
    param.validateBounds<float>(0.5f);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = maxAspectRatioSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum aspect ratio for blob removal.";
    param.type = tp_pipeline::floatSID();
    param.min = 0.0f;
    param.max = 1.0f;
    param.validateBounds<float>(1.0f);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = minDensitySID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The mimimum density of a bounding rect contents for blob removal.";
    param.type = tp_pipeline::intSID();
    param.min = 0.0f;
    param.max = 1.0f;
    param.validateBounds<float>(0.1f);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = maxDensitySID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum density of a bounding rect contents for blob removal.";
    param.type = tp_pipeline::floatSID();
    param.min = 0.0f;
    param.max = 1.0f;
    param.validateBounds<float>(1.0f);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = minSizeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The minimum width or height for blob removal.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 100000;
    param.validateBounds<int>(2);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = maxSizeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum width or height for blob removal.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 100000;
    param.validateBounds<int>(100);

    param.enabled = (mode==Mode_lt::BlackBlobRemoval   ||
                     mode==Mode_lt::WhiteBlobRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = knobletWidthSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum width of knoblets to remove.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 20;
    param.validateBounds<int>(2);

    param.enabled = (mode==Mode_lt::BlackKnobletRemoval ||
                     mode==Mode_lt::WhiteKnobletRemoval);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
