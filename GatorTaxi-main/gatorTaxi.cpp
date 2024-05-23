#include <iostream>
#include <fstream>
#include <regex>
#include <cassert>
#include <numeric>
#include <string>
#include<algorithm>
using namespace std;
//defining structure for the ride 
//we can conviniently structurize the rideNode which can be used for heap and rbtree
typedef struct rideNode {
  //pointers for the left child and right child.
  struct rideNode *leftChild, *rightChild;
  int rideNumber;
  int rideCost;//all the values of the ride needed.
  int tripDuration;
  int indexInHeapVector; 
  enum { Red, Black } nodeColor;//color for the red black tree
  rideNode(int rideId, int cost, int duration) {//initializing the values to the node.
    leftChild = nullptr;
    rightChild = nullptr;
    nodeColor = Red;
    rideNumber = rideId;
    rideCost = cost;
    tripDuration = duration;
  }//checking the color of the node
  bool isRed() { return nodeColor == Red; }
  void swapIndices(rideNode* other) {//swapping the node heap indexes
    int temp = indexInHeapVector;
    indexInHeapVector = other->indexInHeapVector;
    other->indexInHeapVector = temp;
  }//finding the minimum in 2 nodes and comparing based on the given conditions
  bool minimum(rideNode* other) {
    if (rideCost == other->rideCost) {
      return tripDuration < other->tripDuration;
    }
    return rideCost < other->rideCost;
  }
} rideNode;

class minheap {
 public:
  vector<rideNode*> heap_vector;
  int leng;
  //constructer for the min heap to initialize a vector to store the heap
  minheap() {
    heap_vector = vector<rideNode*>(2002, nullptr);
    leng = 0;
  }//inserting a ridenode into the heap
   void insert(rideNode* x) {
    if (leng == -1) {
      leng = 1;
    } else {
      ++leng;
    }
    heap_vector[leng] = x;
    x->indexInHeapVector = leng;
    heapUp(leng);
  }//deleting the node by index
  void deletenodefromindex(int ex) {
    swapn(ex, leng--);
    heap_vector[leng + 1] = nullptr;
    heapDown(ex);
    heapUp(leng);
  }//update the heap by index
   void updateByIndex(int ex) {
    heapDown(ex);
  }//heapify the heap upwards to the root
  void heapUp(int x) {
    while (x > 1 && minimum(x, x / 2)) {
      swapn(x, x / 2);
      x /= 2;
    }
  }//comparator function.
  bool minimum(int a, int b) {
    // returns true if heap[a] < heap[b];
    return heap_vector[a]->minimum(heap_vector[b]);
  }//heapify the heap downwards to the root.
  void heapDown(int x) {
    while (2 * x <= leng) {
      int y = 2 * x;  // leftChild 
      if (y < leng && minimum(y + 1, y))
        ++y;
      if (minimum(x, y))
        break;
      swapn(x, y);
      x = y;
    }
  }//removing the root which has the minimum ride cost
  rideNode* removeminridecost() {
    if (leng == 0) {
      return nullptr;
    }

    rideNode* ret = heap_vector[1];
    swapn(1, leng);
    heap_vector[leng] = nullptr;
    --leng;
    heapDown(1);
    return ret;
  }//replacing the nodes.
  void replace(int ex, rideNode *z) {

    heap_vector[ex] = z;

  }//swapping nodes
  void swapn(int a, int b) {
    swap(heap_vector[a], heap_vector[b]);
    heap_vector[a]->swapIndices(heap_vector[b]);
  }
};

class redblacktree {
 public:
  rideNode* root;
  bool failure;
  int replace_index;
  rideNode * replace_node;
  redblacktree() : root(nullptr) {}
  //when a node with same ride number is tried to insert. 
  bool insert(rideNode* z) {
    root = insert(root, z);
    root->nodeColor = rideNode::Black;
    if (failure) {
      failure = false;
      return false;
    }
    return true;
  }//checking the color of the node
  bool isRed(rideNode* x) {
    if (x == nullptr) {
      return false;
    }
    return x->nodeColor == rideNode::Red;
  }//matching colors of the nodes
  void colormatch(rideNode* e) {
    if (e == nullptr){
      return;
    }
     if(e->leftChild == nullptr)
     {
      return;
     } 
     if(e->leftChild == nullptr) {
      return;
    }
    change(e);
    change(e->leftChild);
    change(e->rightChild);
  }//inserting node color
   rideNode* insert(rideNode* x, rideNode* z) {
    bool should_set_color = x == nullptr;
    if (should_set_color) {
      z->nodeColor = rideNode::Red;
      return z;
    }

   bool should_return_x = x->rideNumber == z->rideNumber;
    if (should_return_x) {
      failure = true;
      return x;
    }

    bool should_insert_in_right_child = x->rideNumber < z->rideNumber;
    if (should_insert_in_right_child) {
      x->rightChild = insert(x->rightChild, z);
    } else {
      x->leftChild = insert(x->leftChild, z);
    }
    return re_balance(x);
  }//changing the color of the node whenever required.
  void change(rideNode* e) {
    if (e == nullptr) {
      return;
    }
    if (e->nodeColor == rideNode::Red) {
      e->nodeColor = rideNode::Black;
    } else {
      e->nodeColor = rideNode::Red;
    }
  }//
  rideNode* moveLeftChild(rideNode* z) {
    if (!(z != nullptr)){ 
      if(z->leftChild != nullptr){
         if((z->rightChild != nullptr)) {
      cout << "bad move leftChild" << endl;
      return z;
    }
    }
    }
    bool is_z_red = isRed(z);
    bool is_left_red = isRed(z->leftChild);
    bool is_right_red = isRed(z->rightChild);

    if ((!((!is_z_red && is_left_red && is_right_red) ||
        (is_z_red && !is_left_red && !is_right_red)))) {
      cout << "bad move leftChild" << endl;
      return z;
    }
    colormatch(z);

    bool should_rotate = isRed(z->rightChild->leftChild);
    if (should_rotate) {
      rideNode* new_right_child = rotate_right_child(z->rightChild);
      z->rightChild = new_right_child;
      z = rotateLeftChild(z);
      colormatch(z);
    }
    return z;
  }//rotating the left child
 rideNode* rotateLeftChild(rideNode* z) {
   bool is_valid_node = (z != nullptr) && isRed(z->rightChild);
    if (!is_valid_node) {
      cout << "Bad leftChild rotate" << endl;
      return z;
    }
    rideNode* x = z->rightChild;
    z->rightChild = x->leftChild;
    x->leftChild = z;
    x->nodeColor = z->nodeColor;
    z->nodeColor = rideNode::Red;
    return x;
  }
  rideNode* move_right_child(rideNode* z) {
  bool is_valid_node = (z != nullptr) && isRed(z) && !isRed(z->rightChild) &&
                     !isRed(z->rightChild->leftChild);

    if (!is_valid_node) {
      cout << "bad move rightChild" << endl;
      return z;
    }
    colormatch(z);
    bool should_rotate = isRed(z->leftChild->leftChild);

    if (should_rotate) {
      z = rotate_right_child(z);
      colormatch(z);
    }

    return z;
  }//rotating the right child
  rideNode* rotate_right_child(rideNode* z) {
    bool is_valid_node = (z != nullptr) && isRed(z->leftChild);

    if (!is_valid_node) {
      cout << "Bad rightChild rotate" << endl;
      return z;
    }
    rideNode* x = z->leftChild;
    z->leftChild = x->rightChild;
    x->rightChild = z;
    x->nodeColor = z->nodeColor;
    z->nodeColor = rideNode::Red;
    return x;
  }//rebalancing the tree.
  rideNode* re_balance(rideNode* x) {
    bool should_rotate_1 = isRed(x->rightChild) && !isRed(x->leftChild);
    if (should_rotate_1) {
      x = rotateLeftChild(x);
    }

    bool should_rotate_2 = isRed(x->leftChild) && isRed(x->leftChild->leftChild);
    if (should_rotate_2) {
      x = rotate_right_child(x);
    }
    
    if (isRed(x->leftChild) && isRed(x->rightChild)) {
      colormatch(x);
    }
    return x;
  }//getting the minimum node in the rbtree
  rideNode* min_node(rideNode* z) {
    bool should_return_nullptr = z == nullptr;
    if (should_return_nullptr) {
      cout << "bad min call" << endl;
      return nullptr;
    }
    if (z->leftChild == nullptr) {
      return z;
    }
    return min_node(z->leftChild);
  }//deleting the minimum ride
  rideNode* delete_min(rideNode* z) {
   bool should_replace_index_and_return_nullptr = z->leftChild == nullptr;

    if (should_replace_index_and_return_nullptr) {
      replace_index = z->indexInHeapVector;
      return nullptr;
    }
    
    bool should_move_left_child = !isRed(z->leftChild) && !isRed(z->leftChild->leftChild);
    if (should_move_left_child) {
      z = moveLeftChild(z);
    }

    z->leftChild = delete_min(z->leftChild);
    return re_balance(z);
  }//deleting the node with id
  rideNode* delete_by_id(rideNode* z, int rideId) {
    if (rideId < z->rideNumber) {
      if (!isRed(z->leftChild) && !isRed(z->leftChild->leftChild)) {
        z = moveLeftChild(z);
      }
      z->leftChild = delete_by_id(z->leftChild, rideId);
    } else {
      if (isRed(z->leftChild)) {
        z = rotate_right_child(z);
      }
      if (z->rightChild == nullptr && z->rideNumber == rideId) {
        replace_node = nullptr;
        return nullptr;
      }

      if (!isRed(z->rightChild) && !isRed(z->rightChild->leftChild)) {
        z = move_right_child(z);
      }
      if (rideId == z->rideNumber) {
        rideNode* x = min_node(z->rightChild);
        z->rideCost = x->rideCost;
        z->rideNumber = x->rideNumber;
        z->tripDuration = x->tripDuration;
        z->indexInHeapVector = x->indexInHeapVector;
        replace_index = x->indexInHeapVector;
        replace_node = z;
        z->rightChild = delete_min(z->rightChild);
      } else {
        z->rightChild = delete_by_id(z->rightChild, rideId);
      }
    }
    return re_balance(z);
  }//delete the node by id
  void delete_by_id(int rideId) {
    replace_node = nullptr;
    if (!isRed(root->leftChild) && !isRed(root->rightChild)) {
      root->nodeColor = rideNode::Red;
    }
    root = delete_by_id(root, rideId);
  }//getting the node by id
  rideNode* get_node(int rideId) {
    rideNode* rc = root;
    do {
      if (rc->rideNumber < rideId) {
        rc = rc->rightChild;
      } else if (rc->rideNumber > rideId) {
        rc = rc->leftChild;
      } else {
        return rc;
      }
    } while (rc != nullptr);

    return nullptr;
  }//get nodes in the given range
  void get_nodes_in_range(rideNode* x, int lo, int hi, vector<rideNode*>& res) {
    if (x != nullptr) {
    if (x->rideNumber > hi) {
        get_nodes_in_range(x->leftChild, lo, hi, res);
    } else if (x->rideNumber < lo) {
        get_nodes_in_range(x->rightChild, lo, hi, res);
    } else {
        get_nodes_in_range(x->leftChild, lo, hi, res);
        res.push_back(x);
        get_nodes_in_range(x->rightChild, lo, hi, res);
    }
  }}//getting the vector of the nodes.
  vector<rideNode*> get_nodes(int lo, int hi) {
    vector<rideNode*> res;
    get_nodes_in_range(root, lo, hi, res);
    return res;
  }
};

// Implementation of DS
class HeapRBT : public minheap, public redblacktree {
 public://to print the node we first make the string for the nodes.
  string string_node(rideNode *node, bool newline, bool comma) {
    string s = "(" + to_string(node->rideNumber) + "," + to_string(node->rideCost) +  ","
        + to_string(node->tripDuration) + (comma ? ")," : ")");
    if (newline) {
      s += "\n";
    }
    return s;
  }//to print a single ride
  string print(int rideNumber) {
    rideNode* node = redblacktree::get_node(rideNumber);
    if (node == nullptr) {
      return "(0,0,0)\n";
    }
    return string_node(node, true, false);
  }//to print rides in a given range.
  string print(int ride_number_start, int ride_number_end) {
    vector<rideNode*> nodes =
        redblacktree::get_nodes(ride_number_start, ride_number_end);
    int n = nodes.size();
    if (n == 0) {
      return "(0,0,0)\n";
    }
    rideNode* node;
    string s = "";
    for (rideNode* node : nodes) {
      s += string_node(node, false, node != nodes.back());
    }

    s += "\n";
    return s;
  }//inserting the rides
  string insert(int rideNumber, int rideCost, int tripDuration) {
    rideNode* z = new rideNode(rideNumber, rideCost, tripDuration);
    bool inserted = redblacktree::insert(z);
    if (!inserted) {
      return "Duplicate RideNumber\n";
    }

    minheap::insert(z);
    
    return "true";
  }//getting the next ride
  string get_next_ride() {
    rideNode* ride = minheap::removeminridecost();
    
    if (ride != nullptr) {
        string s = string_node(ride, true, false);
      redblacktree::delete_by_id(ride->rideNumber);

      if (replace_node != nullptr) {
        minheap::replace(redblacktree::replace_index, redblacktree::replace_node);
      }
      return s;
    } else {
      return "No active ride requests\n";
    }
  }//cancelling the ride
  void cancel_ride(int rideNumber) {
    rideNode* ride = redblacktree::get_node(rideNumber);
    if (ride != nullptr) {
      minheap::deletenodefromindex(ride->indexInHeapVector);
      redblacktree::delete_by_id(rideNumber);
      if (replace_node != nullptr) {
        minheap::replace(redblacktree::replace_index, redblacktree::replace_node);
      }

    }
  }//updating the trip.
  void update_trip(int rideNumber, int new_trip_duration) {
    rideNode* ride = redblacktree::get_node(rideNumber);
    if (ride == nullptr) {
      return;
    }
    int td = ride->tripDuration;
    if (new_trip_duration > 2 * td) {
      minheap::deletenodefromindex(ride->indexInHeapVector);

      redblacktree::delete_by_id(rideNumber);
      if (replace_node != nullptr) {
        minheap::replace(redblacktree::replace_index, redblacktree::replace_node);
      }
      return;
    }
    if (new_trip_duration > td) {
      ride->rideCost += 10;
      ride->tripDuration = new_trip_duration;
      minheap::updateByIndex(ride->indexInHeapVector);
    }
    else{
      ride->tripDuration = new_trip_duration;
      minheap::updateByIndex(ride->indexInHeapVector);
    }
  }
};
//main function
int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "input file name" << endl;
    return 0;
  }

  string input_file(argv[1]);
  ifstream in(input_file);
  ofstream out("output_file.txt");
  HeapRBT h_rbt = HeapRBT();//creating an linked class object
  string line;
 
  regex re("[(|)|,| ]");//regex to divide the given inputs to run them.
  //read all inputs from the input file.
  while (getline(in, line)) {
    //tokenising the inputs from the input file.
    sregex_token_iterator beg(line.begin(), line.end(), re, -1), ending;
    vector<string> combine(beg, ending);
    vector<string> inputstring;
    //add all the seperated strings to a single vector.
    for (string x : combine) {
      if (x.length() > 0) {
        inputstring.push_back(x);
      }
    }

    string fn = inputstring[0];
    string s;
    int rideNumber, rideCost, tripDuration, start, end;
    //using switch we can differentiate the inputs and call the appropriate functions.
    switch (fn[0]) {
      case 'I':
        rideNumber = atoi(inputstring[1].c_str());
        rideCost = atoi(inputstring[2].c_str());
        tripDuration = atoi(inputstring[3].c_str());
        s = h_rbt.insert(rideNumber, rideCost, tripDuration);
        if (s != "true") {
          out << s;
          goto ret;
        }
        break;

      case 'P':
        if ((int)inputstring.size() == 2) {
          rideNumber = atoi(inputstring[1].c_str());
          s = h_rbt.print(rideNumber);
        } else {
          start = atoi(inputstring[1].c_str());
          end = atoi(inputstring[2].c_str());
          s = h_rbt.print(start, end);
        }
        out << s;
        break;

      case 'G':
        s = h_rbt.get_next_ride();
        out << s;
        break;

      case 'C':
        rideNumber = atoi(inputstring[1].c_str());
        h_rbt.cancel_ride(rideNumber);
        break;

      case 'U':
        rideNumber = atoi(inputstring[1].c_str());
        tripDuration = atoi(inputstring[2].c_str());
        h_rbt.update_trip(rideNumber, tripDuration);
        break;
    }

    out.flush();
  }
ret:
  in.close();
  out.close();
  return 0;
}