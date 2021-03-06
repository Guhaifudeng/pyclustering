/**
*
* Copyright (C) 2014-2017    Andrei Novikov (pyclustering@yandex.ru)
*
* GNU_PUBLIC_LICENSE
*   pyclustering is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   pyclustering is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef _INTERFACE_CCORE_H_
#define _INTERFACE_CCORE_H_


#include "definitions.hpp"

#include "interface/pyclustering_package.hpp"


typedef struct cluster_representation {
	unsigned int			size;
	unsigned int			* objects;
} cluster_representation;


typedef struct clustering_result {
	unsigned int			size;
	cluster_representation	* clusters;
} clustering_result;


typedef struct dynamic_result {
	unsigned int			size_dynamic;
	unsigned int			size_network;
	double					* times;
	double					** dynamic;
} dynamic_result;


typedef struct tsp_result {
	unsigned int			size;
	double					path_length;
	unsigned int			* objects_sequence;
} tsp_result;


typedef struct tsp_objects {
    unsigned int            size;
    unsigned int            dimention;
    double                  * data;
} tsp_objects;

typedef struct tsp_matrix {
    unsigned int            size;
    double                  ** data;
} tsp_matrix;

/***********************************************************************************************
 *
 * @brief   Free clustering results that have been provided by CCORE to client.
 *
 * @param   (in) pointer            - pointer to clustering results.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void free_clustering_result(clustering_result * pointer);

/***********************************************************************************************
 *
 * @brief   Free dynamic that have been provided by CCORE to client.
 *
 * @param   (in) pointer            - pointer to dynamic.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void free_dynamic_result(dynamic_result * pointer);

extern "C" DECLARATION void free_pyclustering_package(pyclustering_package * package);

extern "C" DECLARATION void * syncpr_create(const unsigned int num_osc, 
                                            const double increase_strength1, 
                                            const double increase_strength2);

extern "C" DECLARATION void syncpr_destroy(const void * pointer_network);

extern "C" DECLARATION unsigned int syncpr_get_size(const void * pointer);

extern "C" DECLARATION void syncpr_train(const void * pointer_network, 
                                         const void * const patterns);

extern "C" DECLARATION void * syncpr_simulate_static(const void * pointer_network, 
                                                     unsigned int steps, 
                                                     const double time, 
                                                     const void * const pattern,
                                                     const unsigned int solver, 
                                                     const bool collect_dynamic);

extern "C" DECLARATION void * syncpr_simulate_dynamic(const void * pointer_network, 
                                                      const void * const pattern, 
                                                      const double order, 
                                                      const unsigned int solver, 
                                                      const bool collect_dynamic, 
                                                      const double step);

extern "C" DECLARATION double syncpr_memory_order(const void * pointer_network, 
                                                  const void * const pattern);

extern "C" DECLARATION void syncpr_dynamic_destroy(const void * pointer);

extern "C" DECLARATION pyclustering_package * syncpr_dynamic_allocate_sync_ensembles(const void * pointer_dynamic, 
                                                                                     const double tolerance);

extern "C" DECLARATION pyclustering_package * syncpr_dynamic_get_time(const void * pointer);

extern "C" DECLARATION pyclustering_package * syncpr_dynamic_get_output(const void * pointer);


/***********************************************************************************************
 *
 * @brief   Create oscillatory network SYNC for cluster analysis.
 *
 * @param   (in) sample                - input data for clustering.
 * @param   (in) connectivity_radius   - connectivity radius between points.
 * @param   (in) enable_conn_weight    - if True - enable mode when strength between oscillators 
 *                                       depends on distance between two oscillators. Otherwise
 *                                       all connection between oscillators have the same strength.
 * @param   (in) initial_phases        - type of initialization of initial phases of oscillators.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void * syncnet_create_network(const data_representation * const sample, 
                                                     const double connectivity_radius, 
                                                     const bool enable_conn_weight, 
                                                     const unsigned int initial_phases);

/***********************************************************************************************
 *
 * @brief   Destroy SyncNet (calls destructor).
 *
 * @param   (in) pointer_network       - pointer to the SyncNet network.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void syncnet_destroy_network(const void * pointer_network);

/***********************************************************************************************
 *
 * @brief   Simulate oscillatory network SYNC until clustering problem is not resolved.
 *
 * @param   (in) order             - order of synchronization that is used as indication for 
 *                                   stopping processing.
 * @param   (in) solver            - specified type of solving diff. equation. 
 * @param   (in) collect_dynamic   - specified requirement to collect whole dynamic of the network.
 *
 * @return  Returns analyser of output dynamic.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void * syncnet_process(const void * pointer_network, 
                                              const double order, 
                                              const unsigned int solver, 
                                              const bool collect_dynamic);

extern "C" DECLARATION void syncnet_analyser_destroy(const void * pointer_analyser);


/***********************************************************************************************
 *
 * @brief   Create oscillatory network HSyncNet (hierarchical Sync) for cluster analysis.
 *
 * @param[in] sample:            Input data for clustering.
 * @param[in] number_clusters:   Number of clusters that should be allocated.
 * @param[in] initial_phases:    Type of initialization of initial phases of oscillators.
 * @param[in] initial_neighbors: Defines initial radius connectivity by calculation average distance 
 *                               to connect specify number of oscillators.
 * @param[in] increase_persent:  Percent of increasing of radius connectivity on each step (input 
 *                               values in range (0.0; 1.0) correspond to (0%; 100%)).
 *
 * @return Pointer of hsyncnet network. Caller should free it by 'hsyncnet_destroy_network'.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void * hsyncnet_create_network(const data_representation * const sample, 
                                                      const unsigned int number_clusters, 
                                                      const unsigned int initial_phases,
                                                      const unsigned int initial_neighbors,
                                                      const double increase_persent);

/***********************************************************************************************
 *
 * @brief   Destroy oscillatory network HSyncNet (calls destructor).
 *
 * @param   (in) pointer_network      - pointer to HSyncNet oscillatory network.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void hsyncnet_destroy_network(const void * pointer_network);

/***********************************************************************************************
 *
 * @brief   Simulate oscillatory network hierarchical SYNC until clustering problem is not resolved.
 *
 * @param   (in) order             - order of synchronization that is used as indication for 
 *                                   stopping processing.
 * @param   (in) solver            - specified type of solving diff. equation. 
 * @param   (in) collect_dynamic   - specified requirement to collect whole dynamic of the network.
 *
 * @return  Return pointer to hsyncnet analyser of output dynamic
 *
 ***********************************************************************************************/
extern "C" DECLARATION void * hsyncnet_process(const void * pointer_network, const double order, const unsigned int solver, const bool collect_dynamic);

extern "C" DECLARATION void hsyncnet_analyser_destroy(const void * pointer_analyser);


/***********************************************************************************************
 *
 * @brief   Creates and runs ant colony algorithm for TSP.
 *
 * @param[in] objects_coord			 - pointer to array with objects.
 * @param[in] ant_colony_parameters  - pointer to parameters of the ant colony algorithm.
 *
 * @return Pointer to allocated TSP result where shortest length is stored with sequence of visited
 *         objects. This pointer should deallocated by client using 'ant_colony_tsp_destroy'.
 *
 * @see ant_colony_tsp_destroy();
 *
 ***********************************************************************************************/
extern "C" DECLARATION tsp_result * ant_colony_tsp_process(const tsp_objects * objects_coord, const void * ant_colony_parameters);

extern "C" DECLARATION tsp_result * ant_colony_tsp_process_by_matrix(const tsp_matrix * objects_coord, const void * ant_colony_parameters);

/***********************************************************************************************
 *
 * @brief   Frees TSP results that is allocated by ant colony algorithm.
 *
 ***********************************************************************************************/
extern "C" DECLARATION void ant_colony_tsp_destroy(const void * pointer);


/**********************
 *
 * Ant clustering algorithm
 *
 * @brief  Run ant clustering algorithm
 *
 *
 */

extern "C" DECLARATION clustering_result * ant_mean_clustering(const data_representation * const sample, const void * p_ant_clustering_params, unsigned int count_clusters);


extern "C" DECLARATION void * legion_create(const unsigned int size, const unsigned int connection_type, const void * const parameters);

extern "C" DECLARATION void legion_destroy(const void * pointer);

extern "C" DECLARATION void * legion_simulate(const void * pointer, 
                                              const unsigned int steps, 
                                              const double time, 
                                              const unsigned int solver, 
                                              const bool collect_dynamic, 
                                              const void * const stimulus);

extern "C" DECLARATION unsigned int legion_get_size(const void * pointer);

extern "C" DECLARATION void legion_dynamic_destroy(const void * pointer);

extern "C" DECLARATION pyclustering_package * legion_dynamic_get_output(const void * pointer);

extern "C" DECLARATION pyclustering_package * legion_dynamic_get_inhibitory_output(const void * pointer);

extern "C" DECLARATION pyclustering_package * legion_dynamic_get_time(const void * pointer);

extern "C" DECLARATION unsigned int legion_dynamic_get_size(const void * pointer);

#endif
