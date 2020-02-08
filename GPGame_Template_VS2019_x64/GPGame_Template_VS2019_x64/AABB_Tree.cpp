// Credit to: JamesRandall : github.com/JamesRandall/SimpleVoxelEngine/blob/master/voxelEngine/src/AABBTree.cpp
// only thing used was the base idea and new features such as shared_ptr and forward_list

#include "AABB_Tree.h"


AABB_Tree::AABB_Tree(unsigned size) : root_node_index(NULL_NODE), node_count(0), next_free_node(0), node_capacity(size), growth_size(size) {
	// Create a list of blank nodes with the last node having a NULL_NODE as it's next_node
	nodes.resize(size);
	for(unsigned i = 0; i < size; i++) {
		AABB_Node& node = nodes[i];
		node.next_node = i + 1;
	}
	nodes[size - 1].next_node = NULL_NODE; // blank last-next node
}

// Creates a new node to store information and returns it's index in the list of nodes
unsigned AABB_Tree::new_node() {
	if(next_free_node == NULL_NODE) {
		// if there is no next node, increase size of tree
		node_capacity += growth_size;
		nodes.resize(node_capacity);

		// Add a set number of nodes onto the current list of nodes
		for(unsigned i = node_count; i < node_capacity; i++) {
			AABB_Node& node = nodes[i];
			node.next_node = i + 1;
		}
		nodes[node_capacity - 1].next_node = NULL_NODE; // blank last-next node
		next_free_node = node_count;
	}

	// Setup the allocated node in next free node
	unsigned node_index = next_free_node;
	AABB_Node& node = nodes[node_index];
	node.parent_node_index = NULL_NODE;
	node.left_node_index = NULL_NODE;
	node.right_node_index = NULL_NODE;
	next_free_node = node.next_node;
	node_count++;

	return node_index;
}

void AABB_Tree::deallocate_node(unsigned index) {
	// Unallocate memory and set the next free node to what the currect index is free the node
	AABB_Node& node = nodes[index];
	node.next_node = next_free_node;
	next_free_node = index;
	node_count--;
}

// Inserts node and its children depending on where best it will fit using insert_leaf() method
void AABB_Tree::insert(const std::shared_ptr<I_AABB>& obj) {
	// Create new node
	unsigned index = new_node();
	AABB_Node& node = nodes[index];

	// Fill new node
	node.aabb = obj->get_AABB();
	node.object = obj;

	// Insert node
	insert_leaf(index);
	node_index_map[obj] = index;
}

void AABB_Tree::remove(const std::shared_ptr<I_AABB>& obj) {
	// Gets the index of the node
	unsigned index = node_index_map[obj];

	// Removes everything connected to node
	remove_leaf(index);

	// Deallocated the memory of the node itself
	deallocate_node(index);

	// Erases the node from the main list
	node_index_map.erase(obj);
}

void AABB_Tree::update(const std::shared_ptr<I_AABB>& obj) {
	// Get node index and update based upon this and it's AABB object
	unsigned index = node_index_map[obj];
	update_leaf(index, obj->get_AABB());
}

// TODO: change to vector maybe as forward_list is not used anywhere else and harder to explain than std::vector
std::vector<std::shared_ptr<I_AABB>> AABB_Tree::check_overlaps(const std::shared_ptr<I_AABB>& obj) const {
	// Setup empty linked list of shared_pointers
	std::vector<std::shared_ptr<I_AABB>> overlaps;
	std::stack<unsigned> stack;

	// get the AABB of the given object
	AABB test_aabb = obj->get_AABB();

	stack.push(root_node_index);

	// Recursively test the entire tree depending on if it's a leaf or is obj
	while(!stack.empty()) {
		unsigned index = stack.top();
		stack.pop();

		// index will be the current node, if it is null it cannot be overlapping with obj
		if(index == NULL_NODE) { continue; }

		// get the current nodes index
		const AABB_Node& node = nodes[index];

		// check if the node overlaps with the current 
		// Obviously if it doesn't overlap with root nothing happens
		if(node.aabb.overlaps(test_aabb)) {

			// if the current overlap is a leaf and not the current object
			// add the node to the overlaps list
			if(node.is_leaf() && node.object != obj) {
				overlaps.push_back(node.object);
			} else {
				// if it is not the leaf add the nodes children to the stack to check
				stack.push(node.left_node_index);
				stack.push(node.right_node_index);
			}
		}
	}
	return overlaps;
}

void AABB_Tree::insert_leaf(unsigned leaf_index) {

	// if tree is empty add leaf node at root
	if(root_node_index == NULL_NODE) { root_node_index = leaf_index; return; }

	// search for the best location to put the leaf based upon surface area heuristics
	unsigned tree_node_index = root_node_index;
	AABB_Node& leaf_node = nodes[leaf_index];

	// loop to find the best location to fit the leaf
	while(!nodes[tree_node_index].is_leaf()) {

		// get nodes and their indexs for both the current and its children
		const AABB_Node& tree_node = nodes[tree_node_index];
		unsigned left_index = tree_node.left_node_index;
		unsigned right_index = tree_node.right_node_index;
		const AABB_Node& left_node = nodes[left_index];
		const AABB_Node& right_node = nodes[right_index];

		// combine the AABB together for a overall AABB surface area
		AABB merged_aabb = tree_node.aabb.merge(leaf_node.aabb);

		// get the surface area heuristics of attaching the leaf at current location
		float new_parent_cost = 2 * merged_aabb.surface_area;

		// minimum cost increase of attaching to a child
		float minimum_down_cost = 2 * (merged_aabb.surface_area - tree_node.aabb.surface_area);

		// check the cost of adding the leaf to the left child
		float cost_left = (left_node.is_leaf()) ?
			leaf_node.aabb.merge(left_node.aabb).surface_area + minimum_down_cost :
			cost_left = (leaf_node.aabb.merge(left_node.aabb).surface_area - left_node.aabb.surface_area) + minimum_down_cost;

		// check the cost of adding the leaf to the right child
		float cost_right = (right_node.is_leaf()) ?
			leaf_node.aabb.merge(right_node.aabb).surface_area + minimum_down_cost :
			cost_right = (leaf_node.aabb.merge(right_node.aabb).surface_area - right_node.aabb.surface_area) + minimum_down_cost;

		// if the cost of creating a parent is cheaper than creating a child node, then break and use the found index/node for inserting the leaf
		if(new_parent_cost < cost_left && new_parent_cost < cost_right) { break; }

		// if the cost is cheaper to add the leaf as a child, use the cheapest costing child
		tree_node_index = (cost_left < cost_right) ? left_index : right_index;

	}

	// create new parent node 
	unsigned leaf_sibling_index = tree_node_index;
	AABB_Node& leaf_sibling = nodes[leaf_sibling_index];
	unsigned old_parent_index = leaf_sibling.parent_node_index;
	unsigned new_parent_index = new_node();
	AABB_Node& new_parent = nodes[new_parent_index];
	new_parent.parent_node_index = old_parent_index;
	new_parent.aabb = leaf_node.aabb.merge(leaf_sibling.aabb);
	new_parent.left_node_index = leaf_sibling_index;
	new_parent.right_node_index = leaf_index;
	leaf_node.parent_node_index = new_parent_index;

	if(old_parent_index == NULL_NODE) {
		// old parent was the root, so this new node is now the root
		root_node_index = new_parent_index;
	} else {
		// need to assign the left or right index to point to the new parent node
		AABB_Node& old_parent = nodes[old_parent_index];
		if(old_parent.left_node_index == leaf_sibling_index) {
			old_parent.left_node_index = new_parent_index;
		} else {
			old_parent.right_node_index = new_parent_index;
		}
	}

	// walk back up the tree fixing the areas changed by adding the new node
	tree_node_index = leaf_node.parent_node_index;
	fix_tree(tree_node_index);
}

void AABB_Tree::remove_leaf(unsigned leaf_index) {

	// if the leaf is the root node, clear the entire tree
	if(leaf_index == root_node_index) {
		root_node_index = NULL_NODE;
		return;
	}

	// find and set parents of the node with the sibling of the node
	AABB_Node& leaf_node = nodes[leaf_index];
	unsigned parent_node_index = leaf_node.parent_node_index;
	const AABB_Node& parent_node = nodes[parent_node_index];
	unsigned g_parent_node_index = parent_node.parent_node_index;
	unsigned sibling_node_index = (parent_node.left_node_index == leaf_index) ? parent_node.right_node_index : parent_node.left_node_index;
	AABB_Node& sibling_node = nodes[sibling_node_index];

	if(g_parent_node_index != NULL_NODE) {
		// if parent is not the root, remove the parent and attach sibling in its place
		AABB_Node& g_parent_node = nodes[g_parent_node_index];
		if(g_parent_node.left_node_index == parent_node_index) {
			g_parent_node.left_node_index = sibling_node_index;
		} else {
			g_parent_node.right_node_index = sibling_node_index;
		}
		// set sibling parent to g_parent and deallocate current parent node
		sibling_node.parent_node_index = g_parent_node_index;
		deallocate_node(parent_node_index);
		fix_tree(g_parent_node_index);
	} else {
		// if there is no g_parent_node then the parent is root, so the sibling becomes the new root
		root_node_index = sibling_node_index;
		sibling_node.parent_node_index = NULL_NODE;
		deallocate_node(parent_node_index);
	}

	// detach the parent from the leaf node being removed
	leaf_node.parent_node_index = NULL_NODE;
}

void AABB_Tree::update_leaf(unsigned leaf_index, const AABB& new_aabb) {
	// get the node for the leaf
	AABB_Node& node = nodes[leaf_index];
	
	// if the new aabb contains the aabb return
	if(node.aabb.contains(new_aabb)) { return; }

	remove_leaf(leaf_index);
	node.aabb = new_aabb;
	insert_leaf(leaf_index);
}

void AABB_Tree::fix_tree(unsigned tree_node_index) {
	while(tree_node_index != NULL_NODE) {
		// get the node for the given index
		AABB_Node& tree_node = nodes[tree_node_index];

		// fix the height and area
		const AABB_Node& left_node = nodes[tree_node.left_node_index];
		const AABB_Node& right_node = nodes[tree_node.right_node_index];
		tree_node.aabb = left_node.aabb.merge(right_node.aabb);

		tree_node_index = tree_node.parent_node_index;
	}
}

AABB_Tree::~AABB_Tree() {

}