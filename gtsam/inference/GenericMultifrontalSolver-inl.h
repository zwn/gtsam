/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    GenericMultifrontalSolver-inl.h
 * @author  Richard Roberts
 * @date    Oct 21, 2010
 */

#pragma once

#include <gtsam/inference/GenericMultifrontalSolver.h>
#include <gtsam/inference/Factor-inl.h>
#include <gtsam/inference/JunctionTree-inl.h>
#include <gtsam/inference/BayesNet-inl.h>

using namespace std;

namespace gtsam {

	/* ************************************************************************* */
	template<class F, class JT>
	GenericMultifrontalSolver<F, JT>::GenericMultifrontalSolver(
			const FactorGraph<F>& graph) :
			structure_(new VariableIndex(graph)), junctionTree_(
					new JT(graph, *structure_)) {
	}

	/* ************************************************************************* */
	template<class F, class JT>
	GenericMultifrontalSolver<F, JT>::GenericMultifrontalSolver(
			const typename FactorGraph<F>::shared_ptr& graph,
			const VariableIndex::shared_ptr& variableIndex) :
			structure_(variableIndex), junctionTree_(new JT(*graph, *structure_)) {
	}

	/* ************************************************************************* */
	template<class F, class JT>
	void GenericMultifrontalSolver<F, JT>::replaceFactors(
			const typename FactorGraph<F>::shared_ptr& graph) {
		junctionTree_.reset(new JT(*graph, *structure_));
	}

	/* ************************************************************************* */
	template<class F, class JT>
	typename JT::BayesTree::shared_ptr GenericMultifrontalSolver<F, JT>::eliminate(
			typename FactorGraph<F>::Eliminate function) const {

		// eliminate junction tree, returns pointer to root
		typename JT::BayesTree::sharedClique root = junctionTree_->eliminate(function);

		// create an empty Bayes tree and insert root clique
		typename JT::BayesTree::shared_ptr bayesTree(new typename JT::BayesTree);
		bayesTree->insert(root);

		// return the Bayes tree
		return bayesTree;
	}

	/* ************************************************************************* */
	template<class F, class JT>
	typename FactorGraph<F>::shared_ptr GenericMultifrontalSolver<F, JT>::jointFactorGraph(
			const std::vector<Index>& js, Eliminate function) const {

		// We currently have code written only for computing the

		if (js.size() != 2) throw domain_error(
				"*MultifrontalSolver::joint(js) currently can only compute joint marginals\n"
						"for exactly two variables.  You can call marginal to compute the\n"
						"marginal for one variable.  *SequentialSolver::joint(js) can compute the\n"
						"joint marginal over any number of variables, so use that if necessary.\n");

		return eliminate(function)->joint(js[0], js[1], function);
	}

	/* ************************************************************************* */
	template<class F, class JT>
	typename F::shared_ptr GenericMultifrontalSolver<F, JT>::marginalFactor(
			Index j, Eliminate function) const {
		return eliminate(function)->marginalFactor(j, function);
	}

}

