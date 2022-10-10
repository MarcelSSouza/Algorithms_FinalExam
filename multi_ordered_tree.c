//
// AED, January 2022
//
// Solution of the second practical assignement (multi-ordered tree)
//
// Place your student numbers and names here
// Bruno Moura       97151
// Marcus Peterson   101082
// Marcel Souza      101043

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AED_2021_A02.h"

//
// the custom tree node structure
//
// we want to maintain three ordered trees (using the same nodes!), so we need three left and three right pointers
// so, when inserting a new node we need to do it three times (one for each index), so we will end upo with 3 three roots
//

typedef struct tree_node_s
{
  char name[MAX_NAME_SIZE + 1];                         // index 0 data item
  char zip_code[MAX_ZIP_CODE_SIZE + 1];                 // index 1 data item
  char telephone_number[MAX_TELEPHONE_NUMBER_SIZE + 1]; // index 2 data item
  char age[3];                                          // index 3 data item
  struct tree_node_s *left[4];                          // left pointers (one for each index) ---- left means smaller
  struct tree_node_s *right[4];                         // right pointers (one for each index) --- right means larger
}
tree_node_t;

//
// the node comparison function (do not change this)
//
int compare_tree_nodes(tree_node_t *node1,tree_node_t *node2,int main_idx)
{
  int i,c;

  for(i = 0;i < 4;i++)
  {
    if(main_idx == 0)
      c = strcmp(node1->name,node2->name);
    else if(main_idx == 1)
      c = strcmp(node1->zip_code,node2->zip_code);
    else if(main_idx == 2)
      c = strcmp(node1->telephone_number,node2->telephone_number);
    else
      c = strcmp(node1->age,node2->age);
    if(c != 0)
      return c; // different on this index, so return
    main_idx = (main_idx == 3) ? 0 : main_idx + 1; // advance to the next index
  }
  return 0;
}

//Tree insertion routine
//Taken from lecture notes
void tree_insert(int index, tree_node_t** link, tree_node_t* toAdd)
{
  if ( *link == NULL)
    *link = toAdd;
  else if (compare_tree_nodes(*link,toAdd,index) < 0)
    tree_insert(index,&((*link)->right[index]), toAdd);
  else
    tree_insert(index,&((*link)->left[index]), toAdd);
}

//Tree search routine
tree_node_t* find(int index, tree_node_t** link,tree_node_t* toFind)
{
  if (compare_tree_nodes(*link,toFind,index)==0)
    return *link;
  else if (compare_tree_nodes(*link,toFind,index) > 0)
    return find(index,&((*link)->left[index]),toFind);
  else
    return find(index,&((*link)->right[index]),toFind);

  return NULL;  //Shouldn't return NULL normally
}


//Calculates the tree depth recursivly.
int tree_depth(int index, tree_node_t** root)
{
  //This shouldn't happen normally
  if ( *root == NULL)
    return 0;

  int rHeight = tree_depth(index,&((*root)->right[index]));
  int lHeight = tree_depth(index,&((*root)->left[index]));

  return rHeight > lHeight ? rHeight + 1 : lHeight + 1;
}

//Prints all the four fields on the node.
void PrintNode(tree_node_t* node)
{
  printf("~~~~~~~~~~~~~~~~\n");
  printf("Name : %s\n",node->name);
  printf("ZipCode : %s\n",node->zip_code);
  printf("Telephone Number : %s\n",node->telephone_number);
  printf("Age : %s\n",node->age);
}

//Traverses all the nodes in the tree, from the root down. Prints all the nodes too
//Why does this return an int?
int list(int index, tree_node_t* node)
{
  if (node !=NULL){
    PrintNode(node);
    if (node->left[index] != NULL)
      list(index,node->left[index]);
    if (node->right[index] != NULL)
      list(index,node->right[index]);
  }
  return 1;
}

//Searches the tree for people with a given age.
void RecursiveAgeFinder(tree_node_t* node, char *age, int foundFirst){
  if (node !=NULL){
    //If a match is found...
    if(strcmp(node->age, age) == 0){
      foundFirst = 1; //Flag is as such
      PrintNode(node); //And print it
    }
    //Previous match found but not this time. Stop searching this branch
    else if(foundFirst == 1)
      return;

    //Try searching down the left and right branches if they exist
    if (node->left[3] != NULL)
      RecursiveAgeFinder(node->left[3], age, foundFirst);
    if (node->right[3] != NULL)
      RecursiveAgeFinder(node->right[3], age, foundFirst);
  }
}

//Stepping stone function, just to convert int to string for ease of use.
void FindPersonWithAge(tree_node_t* node, int age){

  if(age < 18 || age > 67){
      printf("Try an age between 18 and 67.\n");
      return;
  }

  printf("Finding all %d yo people...\n", age);
  char str[3];
  sprintf(str, "%d", age);

  RecursiveAgeFinder(node, str, 0);
}

//Entry point
int main(int argc,char **argv)
{
    char *filename = "creationTime2.txt";
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    
    char *filename2 = "searchTime2.txt";
    FILE *fp2 = fopen(filename2, "a");
    if (fp2 == NULL)
    {
        printf("Error opening the file %s", filename2);
        return -1;
    }
    
    char *filename3 = "depthMax2.txt";
    FILE *fp3 = fopen(filename3, "a");
    if (fp3 == NULL)
    {
        printf("Error opening the file %s", filename3);
        return -1;
    }

  double dt;

  // process the command line arguments
  if(argc < 3)
  {
    fprintf(stderr,"Usage: %s student_number number_of_persons [options ...]\n",argv[0]);
    fprintf(stderr,"Recognized options:\n");
    fprintf(stderr,"  -list[N]              # list the tree contents, sorted by key index N (the default is index 0)\n");
    // place a description of your own options here
    return 1;
  }
  int student_number = atoi(argv[1]);
  if(student_number < 1 || student_number >= 1000000)
  {
    fprintf(stderr,"Bad student number (%d) --- must be an integer belonging to [1,1000000{\n",student_number);
    return 1;
  }
  int n_persons = atoi(argv[2]);
  if(n_persons < 3 || n_persons > 10000000)
  {
    fprintf(stderr,"Bad number of persons (%d) --- must be an integer belonging to [3,10000000]\n",n_persons);
    return 1;
  }
  // generate all data
  tree_node_t *persons = (tree_node_t *)calloc((size_t)n_persons,sizeof(tree_node_t));
  if(persons == NULL)
  {
    fprintf(stderr,"Output memory!\n");
    return 1;
  }
  aed_srandom(student_number);
  for(int i = 0;i < n_persons;i++)
  {
    random_name(&(persons[i].name[0]));
    random_zip_code(&(persons[i].zip_code[0]));
    random_telephone_number(&(persons[i].telephone_number[0]));
    random_age(&(persons[i].age[0]));
    for(int j = 0;j < 4;j++)
      persons[i].left[j] = persons[i].right[j] = NULL; // make sure the pointers are initially NULL
  }
  // create the ordered binary trees
  dt = cpu_time();
  tree_node_t *roots[4]; // three indices, three roots
  for(int main_index = 0;main_index < 4;main_index++)
    roots[main_index] = NULL;
  for(int i = 0;i < n_persons;i++)
    for(int main_index = 0;main_index < 4;main_index++)
      //Insert!
       tree_insert(main_index, &roots[main_index], &persons[i]);
  dt = cpu_time() - dt;
  printf("Tree creation time (%d persons): %.3es\n",n_persons,dt);
  fprintf(fp,"%.3e\n",dt);
  fclose(fp);
  // search the tree
  for(int main_index = 0;main_index < 4;main_index++)
  {
    dt = cpu_time();
    for(int i = 0;i < n_persons;i++)
    {
      tree_node_t n = persons[i]; // make a copy of the node data
      //PrintNode(&n);
      if(find(main_index, &roots[main_index], &n) != &(persons[i])) // Find!
      {
        fprintf(stderr,"person %d not found using index %d\n",i,main_index);
        return 1;
      }
    }
    dt = cpu_time() - dt;
    
    printf("Tree search time (%d persons, index %d): %.3es\n",n_persons,main_index,dt);
    fprintf(fp2,"%.3e\t",dt);

    if(main_index==3){
        fprintf(fp2,"\n");
        fclose(fp2);
    }
  }
  // compute the largest tree depdth
  for(int main_index = 0;main_index < 4;main_index++)
  {
    dt = cpu_time();
    int depth = tree_depth(main_index, &roots[main_index]); // place your code here to compute the depth of the tree with number main_index
    dt = cpu_time() - dt;
    printf("Tree depth for index %d: %d (done in %.3e seconds)\n",main_index,depth,dt);
    fprintf(fp3,"%d\t",depth);
    if(main_index==3){
        fprintf(fp3,"\n");
        fclose(fp3);
    }
  }
  
  // process the command line optional arguments
  for(int i = 3;i < argc;i++)
  {
    if(strncmp(argv[i],"-list",5) == 0)
    { // list all (optional)
      int main_index = atoi(&(argv[i][5]));
      if(main_index < 0)
        main_index = 0;
      if(main_index > 2)
        main_index = 2;
      printf("List of persons:\n");
      (void)list( main_index, &persons[main_index] ); // place your code here to traverse, in order, the tree with number main_index
    }
    // place your own options here
  }
  // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
  free(persons);
  return 0;
}
