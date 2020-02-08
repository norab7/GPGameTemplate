// Credit to: JamesRandall : github.com/JamesRandall/SimpleVoxelEngine/blob/master/voxelEngine/src/AABBTree.h
#pragma once
#include <memory>
#include <map>			// standard map, connecting something to something else like a dictionary in other languages
#include <vector>
#include <forward_list> // single-linked-list
#include <stack>

#include "AABB.h"
#include "I_AABB.h"


#define NULL_NODE -1


	// Node structure to save each of the nodes, could be class maybe
struct AABB_Node {
	AABB aabb;
	std::shared_ptr<I_AABB> object; // Creates a pointer shared by multiple places, deallocates self when un-pointed to by any variable

	unsigned parent_node_index;
	unsigned left_node_index;
	unsigned right_node_index;
	unsigned next_node;

	bool is_leaf() const { return left_node_index == NULL_NODE; }
	AABB_Node() : object(nullptr), parent_node_index(NULL_NODE), left_node_index(NULL_NODE), right_node_index(NULL_NODE), next_node(NULL_NODE) {

	}
};

class AABB_Tree {
	std::map<std::shared_ptr<I_AABB>, unsigned> node_index_map; // maps nodes to index values
	std::vector<AABB_Node> nodes;

	unsigned root_node_index;
	unsigned next_free_node;
	unsigned node_capacity;
	unsigned node_count;
	unsigned growth_size;

	unsigned new_node();
	void deallocate_node(unsigned node_index);
	void insert_leaf(unsigned leaf_index);
	void remove_leaf(unsigned leaf_index);
	void update_leaf(unsigned leaf_index, const AABB& new_aabb);
	void fix_tree(unsigned node_index);

public:
	AABB_Tree(unsigned size);
	~AABB_Tree();

	void insert(const std::shared_ptr<I_AABB>& obj);
	void remove(const std::shared_ptr<I_AABB>& obj);
	void update(const std::shared_ptr<I_AABB>& obj);
	std::vector<std::shared_ptr<I_AABB>> check_overlaps(const std::shared_ptr<I_AABB>& obj) const;

};
