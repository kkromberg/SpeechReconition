/*****************************************************************************/
/*                                                                           */
/*       COPYRIGHT (C) 2015 Lehrstuhl fuer Informatik VI, RWTH Aachen        */
/*                                                                           */
/*****************************************************************************/

#ifndef __OUTPUT_LAYER_HPP__
#define __OUTPUT_LAYER_HPP__

#include "FeedForwardLayer.hpp"

class OutputLayer : public FeedForwardLayer {
public:
  OutputLayer(Configuration const& config);
  virtual ~OutputLayer();

  virtual void forward (std::valarray<float>& output, std::gslice const& slice, std::vector<unsigned> const& mask) const;
private:

  float shifted_exp_sum_(const float shift_value, std::valarray<float>& values) const;
  void  log_space_normalization_(const float norm, std::valarray<float>& values) const;
  void  softmax_(std::valarray<float>& values) const;
};

// -------------------- inline functions --------------------

inline OutputLayer::OutputLayer(Configuration const& config) : FeedForwardLayer(config) {
}

inline OutputLayer::~OutputLayer() {
}

#endif /* __OUTPUT_LAYER_HPP__ */
