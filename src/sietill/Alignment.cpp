/*****************************************************************************/
/*                                                                           */
/*       COPYRIGHT (C) 2015 Lehrstuhl fuer Informatik VI, RWTH Aachen        */
/*                                                                           */
/*****************************************************************************/

#include "Alignment.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <limits>

/*****************************************************************************/

namespace {
  // compute -log(exp(-a) + exp(-b)) using the following equality:
  // -log(exp(-a) + exp(-b)) = -log(exp(-a-c) + exp(-b-c)) - c
  //                         = -log(1 + exp(-abs(a-b))) - c
  // where c = max(-a, -b) = -min(a, b)
  double logsum(double a, double b) {
    const double diff = a - b;
    // if a = b = inf then a - b = nan, but for logsum the result is well-defined (inf)
    if (diff != diff) {
      return std::numeric_limits<double>::infinity();
    }
    return -log1p(std::exp(-std::abs(diff))) + std::min(a, b);
  }
}

/*****************************************************************************/

Aligner::Aligner(MixtureModel const& mixtures, TdpModel const& tdp_model, size_t max_aligns)
                 : mixtures_(mixtures), tdp_model_(tdp_model), max_aligns_(max_aligns) {}

/*****************************************************************************/

double Aligner::align_sequence_full(FeatureIter feature_begin, FeatureIter feature_end,
                                    MarkovAutomaton const& reference,
                                    AlignmentIter align_begin, AlignmentIter align_end) {

  // TODO: implement
	CostMatrix cost_matrix;
	BackpointerMatrix backpointer_matrix;
	size_t feature_number = feature_end - feature_begin;
	size_t state_number   = reference.num_states();
	std::cerr << "Number of features: " << feature_number << std::endl;
	// initialise cost and backpointer matrix
	for (StateIdx state = 0; state < state_number; state++){
		cost_matrix.push_back(std::vector<double>(feature_number, std::numeric_limits<double>::infinity())); // set costs to infinity
		backpointer_matrix.push_back(std::vector<size_t>(feature_number, 0));													  	 // backpointer initially with zeros
	}

	double local_costs, loop_costs, forward_costs, skip_costs = std::numeric_limits<double>::infinity();
	size_t n = 0;
	// costs for the first point are fixed
	cost_matrix[0][0] = compute_local_costs( reference[0], *(*feature_begin));

	for (FeatureIter feature_iter = feature_begin; feature_iter != feature_end; feature_iter++, n++) { // loop features
		//TODO determine slope for the current point
		//double result = tdp_model_.score(0, 0);
		for (StateIdx state = 0; state < state_number; state++) { // loop states

			local_costs = compute_local_costs(reference[state], **feature_iter);
			// (t-1) loop costs
			loop_costs    = cost_matrix[state][n];
			if (((state-1) >= 0) && ((n-1) >= 0)){  // avoid out of bounds error
				// (t-1)(s-1) forward_costs
				forward_costs = cost_matrix[state-1][n-1];
				if (state -2 >= 0){ // avoid out of bound error
					// (t-1)(s-2) skip costs
					skip_costs = cost_matrix[state-2][n-1];
				}
			}
			// store minimal costs for current point
			cost_matrix[state][n] = local_costs + std::min(loop_costs,std::min(forward_costs, skip_costs));
		}
	}

	// TODO mapping of automaton states to alignment
	for (AlignmentIter align_iter = align_begin; align_iter != align_end; align_iter++) { // loop
			//(*align_iter)->state = automaton_state
	}

	// return the last entry from the cost matrix
  return cost_matrix[state_number-1][feature_number-1];
}


double Aligner::compute_local_costs(StateIdx state, double feature) {
	double costs = fabs(state - feature);
	return costs;
}
/*****************************************************************************/

double Aligner::align_sequence_pruned(FeatureIter feature_begin, FeatureIter feature_end,
                                      MarkovAutomaton const& reference,
                                      AlignmentIter align_begin, AlignmentIter align_end,
                                      double pruning_threshold) {
  // TODO: implement
  return 0.0;
}

/*****************************************************************************/

void dump_alignment(std::ostream& out, Alignment const& alignment, size_t max_aligns) {
  for (size_t f = 0ul; f < (alignment.size() / max_aligns); f++) {
    for (size_t a = 0ul; a < alignment[f * max_aligns].count; a++) {
      const size_t idx = f * max_aligns + a;
      out << f << " " << alignment[idx].state << " " << alignment[idx].weight << std::endl;
    }
  }
}

/*****************************************************************************/

void write_alignment(std::ostream& out, Alignment const& alignment, size_t max_aligns) {
  out.write(reinterpret_cast<char const*>(&max_aligns), sizeof(max_aligns));
  size_t num_frames = alignment.size() / max_aligns;
  out.write(reinterpret_cast<char const*>(&num_frames), sizeof(num_frames));
  out.write(reinterpret_cast<char const*>(alignment.data()), max_aligns * num_frames * sizeof(Alignment::value_type));
}

/*****************************************************************************/

void read_alignment(std::istream& in, Alignment& alignment, size_t& max_aligns) {
  in.read(reinterpret_cast<char*>(&max_aligns), sizeof(max_aligns));
  size_t num_frames = 0ul;
  in.read(reinterpret_cast<char*>(&num_frames), sizeof(num_frames));
  alignment.resize(num_frames * max_aligns);
  in.read(reinterpret_cast<char*>(alignment.data()), max_aligns * num_frames * sizeof(Alignment::value_type));
}

/*****************************************************************************/
