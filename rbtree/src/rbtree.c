#include <stdio.h>
#include "rbtree.h"


void rbtree_left_rotate(rbtree_t *tree,
		rbtree_node_t *node) {
	
	rbtree_node_t *right = node->right;
	rbtree_node_t *parent = node->parent;
	rbtree_node_t *right_left = right->left;

	right->parent = parent;
	if (parent != tree->sentinel) {
		// node is not the tree root
		if (parent->left == node) {
			// node is left of parent
			parent->left = right;
		} else {
			// node is right of parent
			parent->right = right;
		}
	} else {
		// node is the root
		tree->root = right;
	}

	right->left = node;
	node->parent = right;
	
	node->right = right_left;
	if (right_left != tree->sentinel)
		right_left->parent = node;
}

void rbtree_right_rotate(rbtree_t *tree, 
		rbtree_node_t *node) {
	rbtree_node_t *left = node->left;
	rbtree_node_t *parent = node->parent;
	rbtree_node_t *left_right = left->right;

	left->parent = parent;
	if (parent != tree->sentinel) {
		// node is not the tree root
		if (parent->left == node) {
			parent->left = left;
		} else {
			parent->right = left;
		}
	} else {
		// node is root of the tree
		tree->root = left;
	}

	left->right = node;
	node->parent = left;

	node->left = left_right;
	if (left_right != tree->sentinel)
		left_right->parent = node;
}

rbtree_node_t *rbtree_predecessor(rbtree_t *tree, rbtree_node_t *node) {
	if (node->left != tree->sentinel) {
		// 存在左孩子,那么node的前驱节点为左节点的最右节点
		node = node->left;
		while (node->right != tree->sentinel) {
			node = node->right;
		}
		return node;
	} else if (node->parent != tree->sentinel) {
		// node没有左孩子，此时父节点不为空，并且node为父节点的右孩子，
		// 那么此时父节点就是node的前驱节点
		if (node == node->parent->right) {
			return node->parent;
		} else {
			// node没有左孩子，并且node为父节点的左孩子，
			// 此时node的前驱节点为其向上查找的第一个具有如下特点的
			// 祖先节点：node在这个祖先节点的右子树上。如果不存在这样的
			// 祖先节点，说明node没有前驱节点。
			while(node->parent != tree->sentinel) {
				if (node == node->parent->right) {
					return node->parent;
				}
				node = node->parent;
			}
			return tree->sentinel;
		}
	} 
	// node既没有左孩子，也没有后继父节点
	return tree->sentinel;
}

rbtree_node_t *rbtree_successor(rbtree_t *tree, rbtree_node_t *node) {
	// node存在右孩子，则右孩子的最左孩子即为node的后继
	if (node->right != tree->sentinel) {
		node = node->right;
		while (node->left != tree->sentinel) {
			node = node->left;
		}
		return node;
	} else if (node->parent != tree->sentinel) {
		// node不存在右孩子，并且父亲节点不为空，那么node的后继节点是
		// 第一个有如下特点的祖先节点：
		// node在这个祖先节点的左子树上
		while (node->parent != tree->sentinel) {
			if (node == node->parent->left) {
				return node->parent;
			}
			node = node->parent;
		}
	}
	// node既没有右孩子，也没有后继父节点
	return tree->sentinel;
}

static inline void rbtree_insert_fixup(rbtree_t *tree, rbtree_node_t *z) {
	while (1) {
		if (z == tree->root) {
			// z为根节点
			rbtree_black(z);
			return;
		} else if (rbtree_is_black(z->parent)) {
			// z的父节点为黑色的，不影响红黑树的性质，直接返回
			return;
		} else {
			// z的父节点为红色
			rbtree_node_t *p = z->parent;
			rbtree_node_t *g = p->parent;
			if (g->left == p) {
				// 父亲节点为祖父节点的左孩子

				// 父亲节点为红色，那么祖父节点一定是黑色
				rbtree_node_t *u = g->right;
				if (rbtree_is_red(u)) {
					// 叔叔节点为红色
					// 当前处于z节点为红色，p节点为红色，u（叔叔节点）节点为红色，祖父节点为黑色
					// 那么就可以将祖父节点g设置为红色，p和u设置为黑色，再将g当做z继续向上处理
					rbtree_red(g);
					rbtree_black(u);
					rbtree_black(p);
					z = g;
				} else {
					// 叔叔节点为黑色
					// 当前处于z为红色，p为红色，u（叔叔节点）为黑色，祖父节点为黑色

					// 如果z时p的右孩子，则将p左旋，并p节点变成z节点，z节点变为p节点，颜色不变
					// 这样得到z是p的左孩子
					if (z == p->right) {
						rbtree_left_rotate(tree, p);
						z = p;
						p = z->parent;
					}
					// z是p的左孩子，并且z为红色，p为红色，u为黑色，祖父为黑色
					// 那么可将祖父节点右旋，然后将祖父节点g设置为红色，p设置为黑色
					rbtree_right_rotate(tree, g);
					rbtree_red(g);
					rbtree_black(p);
				}
			} else {
				// 父亲节点为祖父节点的右孩子

				// 父亲节点为红色，那么祖父节点一定是黑色
                rbtree_node_t *u = g->left;
                if (rbtree_is_red(u)) {
                    // 叔叔节点为红色
                    // 当前处于z节点为红色，p节点为红色，u（叔叔节点）节点为红色，祖父节点为黑色
                    // 那么就可以将祖父节点g设置为红色，p和u设置为黑色，再将g当做z继续向上处理
                    rbtree_red(g);
                    rbtree_black(u);
                    rbtree_black(p);
                    z = g;
                } else {
                    // 叔叔节点为黑色
                    // 当前处于z为红色，p为红色，u（叔叔节点）为黑色，祖父节点为黑色

                    // 如果z时p的右孩子，则将p右旋，并p节点变成z节点，z节点变为p节点，颜色不变
                    // 这样得到z是p的左孩子
                    if (z == p->left) {
                        rbtree_right_rotate(tree, p);
                        z = p;
                        p = z->parent;
                    }   
                    // z是p的左孩子，并且z为红色，p为红色，u为黑色，祖父为黑色
                    // 那么可将祖父节点左旋，然后将祖父节点g设置为红色，p设置为黑色
                    rbtree_left_rotate(tree, g);
                    rbtree_red(g);
                    rbtree_black(p);
                }
			}
		}
	}
}

void rbtree_insert(rbtree_t *tree, rbtree_node_t *z) {
	rbtree_node_t *y = tree->sentinel;
	rbtree_node_t *x = tree->root;
	// 找到key值第一个小于z->key的节点，
	// 即y节点为小于z的最大节点，x为nil的节点
	while (x != tree->sentinel) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	if (y == tree->sentinel) {
		// 当前树为空
		tree->root = z;
		z->parent = tree->sentinel;
	} else if (y->key > z->key) {
		y->left = z;
		z->parent = y;
	} else {
		// y->key <= z->key
		y->right = z;
		z->parent = y;
	}
	z->left = z->right = tree->sentinel;
	rbtree_red(z);
	// 修复红黑树的性质
	rbtree_insert_fixup(tree, z);
}

// 用节点v替换节点u
static void transplant(rbtree_t *tree, rbtree_node_t *u, rbtree_node_t *v) {
	if (u->parent == tree->sentinel) {
		tree->root = v;
	} else if (u == u->parent->left) {
		// u为其父节点的左孩子
		u->parent->left = v;
	} else {
		// u为其父节点的右孩子
		u->parent->right = v;
	}

//	if (v != tree->sentinel) {
		// 当v为sentinel时，也需要设置sentinel的parent为u的父节点，这是由于后面得通过这个sentinel节点找到他的父节点
		v->parent = u->parent;
//	}
}

static inline void rbtree_delete_fixup(rbtree_t *tree, rbtree_node_t *x) {
	while (x != tree->root && !rbtree_is_red(x)) {
		// x 非根（如果是根，直接将根的颜色直接改为黑色即可）
		// 并且x不是红色（如果是红色，直接将红色改为黑色即可）

		// 此时x所在分支少一个黑色节点
		rbtree_node_t *p = x->parent;
		
		if (x == p->left) {
			// x为p的左孩子
			rbtree_node_t *s = p->right;
			rbtree_node_t *g = p->parent;
			if (rbtree_is_red(s)) {
				// 兄弟节点为红色，将父节点p左旋，p的颜色设置为红色，s的颜色设置为黑色，
				// 此时x的分支依然缺少一个黑色节点,接着继续其他情况的判断
				rbtree_left_rotate(tree, p);
				rbtree_red(p);
				rbtree_black(s);
				continue;
			} else {
				// 兄弟节点的颜色为黑色
				rbtree_node_t *sr = s->right;
				rbtree_node_t *sl = s->left;
				if (rbtree_is_red(sl) && rbtree_is_black(sr)) {
					// 兄弟节点的左孩子为红色，右孩子为黑色
					// 将s右旋，然后将s的颜色设置为红色，sl的颜色设置为黑色，并且重新设置x的s、sl和sr指针
					rbtree_right_rotate(tree, s);
					rbtree_red(s);
					rbtree_black(sl);
					s = p->right;
					sr = s->right;
					sl = s->left;
				}
				
				if (rbtree_is_red(sr)) {
					// 兄弟节点的右孩子为红色, 左孩子颜色不重要
					// 将p左旋，然后s和p的颜色互换，sr的颜色设置为黑色，然后直接退出
					rbtree_left_rotate(tree, p);
					unsigned p_color = p->color;
					rbtree_copy_color(p, s);
					s->color = p_color;
					rbtree_black(sr);
					return;
				} 

				if (rbtree_is_black(sl) && rbtree_is_black(sr)) {
					// 兄弟节点的左右孩子都为黑色
					if (rbtree_is_red(p)) {
						// p为红色，s为黑色，sl和sr都为黑色，则将s设置为红色，p设置为黑色，然后返回
						rbtree_red(s);
						rbtree_black(p);
						return;
					}
					// 此时将s设置为红色，p当做x，然后继续向上处理
					rbtree_red(s);
					x = p;
				}
			}

		} else {
			// x为p的右孩子
            rbtree_node_t *s = p->left;
            rbtree_node_t *g = p->parent;
            if (rbtree_is_red(s)) {
                // 兄弟节点为红色，将父节点p右旋，p的颜色设置为红色，s的颜色设置为黑色，
                // 此时x分支依然少一个黑色节点，继续其他情况的分析
                rbtree_right_rotate(tree, p); 
                rbtree_red(p);
                rbtree_black(s);
              	continue;
            } else {
                // 兄弟节点的颜色为黑色
                rbtree_node_t *sr = s->right;
                rbtree_node_t *sl = s->left;
                if (rbtree_is_red(sr) && rbtree_is_black(sl)) {
                    // 兄弟节点的右孩子为红色，左孩子为黑色
                    // 将s左旋，然后将s的颜色设置为红色，sr的颜色设置为黑色，并且重新设置x的s、sl和sr指针
                    rbtree_left_rotate(tree, s); 
                    rbtree_red(s);
                    rbtree_black(sr);
                    s = p->left;
                    sr = s->right;
                    sl = s->left;
                }   
                    
                if (rbtree_is_red(sl)) {
                    // 兄弟节点的左孩子为红色, 右孩子颜色不重要
                    // 将p右旋，然后s和p的颜色互换，sl的颜色设置为黑色，然后直接退出
                    rbtree_right_rotate(tree, p); 
                    unsigned p_color = p->color;
                    rbtree_copy_color(p, s); 
                    s->color = p_color;
                    rbtree_black(sl);
                    return;
                }   

                if (rbtree_is_black(sl) && rbtree_is_black(sr)) {
                    // 兄弟节点的左右孩子都为黑色
					if (rbtree_is_red(p)) {
						// p为红色，s为黑色，sl和sr为黑色，则设置s为红色，p设置为黑色，然后返回
						rbtree_black(p);
						rbtree_red(s);
						return;
					}
                    // p为黑色，此时将s设置为红色，p当做x，然后继续向上处理
                    rbtree_red(s);
                    x = p;
                }
            }

		}
	}
	rbtree_black(x);
}

void rbtree_delete(rbtree_t *tree, rbtree_node_t *z) {
	rbtree_node_t *y = z;
	rbtree_node_t *x = tree->sentinel;
	unsigned y_original_color = y->color;
	if (z->left == tree->sentinel) {
		// z的右孩子代替z的位置
		x = z->right;
		transplant(tree, z, z->right);
	} else if (z->right == tree->sentinel) {
		// z的左孩子代替z的位置
		x = z->left;
		transplant(tree, z, z->left);
	} else {
		// 使用z的后继代替z的位置
		y = rbtree_successor(tree, z);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z) {
			// y是z的子节点, x为y的右孩子,
			// 这里当x为sentinel时，可能需要暂时设置sentinel的parent为y，
			// 因为后面在进行delete_fixup时需要通过x->parent向上查找，
			// 如果没有这个指针，那么查找parent就会失败，红黑树的调整也将出错
			// 也就是当x为sentinel时，必须设置sentinel的parent为y（一切为了delete_fixup处理方便）
			x->parent = y;
		} else {
			// y不是z的子节点
			transplant(tree, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(tree, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	// 如果y原来的颜色是黑色，需要对树进行调整
	if (y_original_color == RBTREE_COLOR_BLACK) {
		rbtree_delete_fixup(tree, x);
	}
}

