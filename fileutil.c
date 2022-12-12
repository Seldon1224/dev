#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

#define RED 1
#define BLACK 0
class node
{
public:
	int data;
	node *rchild, *lchild;
	node *parent;
	bool color; // 0=black,1=red;
};
class tree
{
public:
	node *root;
	node *nil;
	tree()
	{
		root = nullptr;
	}

	void insert(const int &a)
	{
		node *&bt = root;
		node *s = new node;
		s->data = a;
		s->color = true; // ²åÈë½ÚµãÎªºìÉ«
		s->lchild = nullptr;
		s->rchild = nullptr;
		s->parent = nullptr;
		// node* &bt = root;
		if (bt == nullptr)
		{
			bt = s;
			bt->color = false;
			bt->parent = bt;
			bt->lchild = nullptr;
			bt->rchild = nullptr;
		}
		else
		{
			// ÕÒµ½Òª²åÈëµÄÎ»ÖÃ
			node *curr = root;
			while (curr->lchild != nullptr | curr->rchild != nullptr)
			{
				if (a < curr->data)
					curr = curr->lchild;
				else
					curr = curr->rchild;
			}

			// ²åÈës½Úµã
			s->parent = curr;
			if (a < curr->data)
				curr->lchild = s;
			else
				curr->rchild = s;

			// ¸üÐÂºìºÚÊ÷£¬±£Ö¤Âú×ãÌØÐÔ
			while (curr->color == RED && curr->parent != nullptr)
			{
				bool isRight = (curr == curr->parent->rchild);
				node *uncle;
				if (isRight)
					uncle = curr->parent->lchild;
				else
					uncle = curr->parent->rchild;

				if (uncle == nullptr) // Èç¹ûuncle½ÚµãÎª¿Õ
				{
					curr->color = BLACK;
					curr->parent->color = RED;
					if (uncle == curr->parent->rchild)
					{
						rightRotate(curr->parent);
					}
					else
					{
						leftRotate(curr->parent);
					}
					break;
				}
				else if (uncle->color == RED) // Èç¹ûuncle½ÚµãÊÇºì
				{
					curr->color = BLACK;
					uncle->color = BLACK;
					curr->parent->color = RED;
					curr = curr->parent;
				}
				else
				{
					curr->color = BLACK;
					curr->parent->color = RED;

					if (isRight)
					{
						if (s == curr->lchild)
						{
							rightRotate(curr);
							curr = s;
						}
						leftRotate(curr->parent);
					}
					else
					{
						if (s == curr->rchild)
						{
							leftRotate(curr);
							curr = s;
						}
						rightRotate(curr->parent);
					}
				}
				root->color = BLACK;
			}
		}
	}

	bool remove(const int &key)
	{
		// ÕÒµ½ÒªÉ¾³ýµÄ½ÚµãÎ»ÖÃ
		node *curr = root;
		while (curr->lchild != nullptr | curr->rchild != nullptr)
		{
			if (curr->data == key)
				break;

			if (key >= curr->data)
				curr = curr->rchild;
			else
				curr = curr->lchild;
		}
		if (curr->data != key)
			return 0;

		this->removeNode(curr);
		return 1;
	}

	void removeNode(node *rbnode)
	{
		if (rbnode->color == RED)
		{
			if (rbnode->lchild != nullptr && rbnode->rchild != nullptr)
			{
				// rbnodeÖ»ÓÐlchildÒÔ¼°rchild
				// ÕÒµ½×î×óµÄºó¼Ì½Úµã£¬ÓÃ´Ë½ÚµãÌæ»»rbnode
				node *successor = rbnode->rchild;
				while (successor->lchild != nullptr)
					successor = successor->lchild;
				rbnode->data = successor->data;
				this->removeNode(successor);
			}
			else if (rbnode->lchild != nullptr)
			{
				// rbnodeÖ»ÓÐlchildÊ±
				rbnode->data = rbnode->lchild->data;
				rbnode->color = rbnode->lchild->color;
				this->removeNode(rbnode->lchild);
			}
			else if (rbnode->rchild != nullptr)
			{
				// rbnodeÖ»ÓÐrchildÊ±
				rbnode->data = rbnode->rchild->data;
				rbnode->color = rbnode->rchild->color;
				this->removeNode(rbnode->rchild);
			}
			else
			{
				// rbnodeÃ»ÓÐº¢×Ó½Úµã
				if (rbnode->parent == nullptr)
				{
					free(rbnode);
					root = nullptr;
					return;
				}

				if (rbnode->parent->lchild == rbnode)
					rbnode->parent->lchild = nullptr;
				else
					rbnode->parent->rchild = nullptr;

				free(rbnode);
			}
		}
		else
		{
			if (rbnode->lchild != nullptr && rbnode->rchild != nullptr)
			{
				// rbnodeÖ»ÓÐlchildÒÔ¼°rchild
				// ÕÒµ½×î×óµÄºó¼Ì½Úµã£¬ÓÃ´Ë½ÚµãÌæ»»rbnode
				node *successor = rbnode->rchild;
				while (successor->lchild != nullptr)
					successor = successor->lchild;
				rbnode->data = successor->data;
				this->removeNode(successor);
			}
			else if (rbnode->lchild != nullptr)
			{
				// rbnodeÖ»ÓÐlchildÊ±
				rbnode->data = rbnode->lchild->data;
				this->removeNode(rbnode->lchild);
			}
			else if (rbnode->rchild != nullptr)
			{
				// rbnodeÖ»ÓÐrchildÊ±
				rbnode->data = rbnode->rchild->data;
				this->removeNode(rbnode->rchild);
			}
			else
			{
				// rbnodeÃ»ÓÐº¢×Ó½Úµã£¬¼´ÎªÒ¶×Ó½Úµã
				// ×÷ÎªÑÕÉ«Îªblack½Úµã£¬ÐèÒªfixupÐÞÕý£¬±£Ö¤ºìºÚÊ÷ÌØÐÔ
				if (rbnode->parent == nullptr)
				{
					// 'rbnode'ÎªRoot
					free(rbnode);
					root = nullptr;
					return;
				}
				// rbnodeÊÇ¸¸½ÚµãµÄ×óº¢×Ó
				if (rbnode->parent->lchild == rbnode)
				{
					rbnode->parent->lchild = nullptr;
					if (rbnode->parent->rchild != nullptr && rbnode->parent->rchild->color == RED)
					{
						rbnode->parent->rchild->color = BLACK;
						leftRotate(rbnode->parent);
					}
				}
				else // rbnodeÊÇ¸¸½ÚµãµÄÓÒº¢×Ó
				{
					rbnode->parent->rchild = nullptr;
					if (rbnode->parent->lchild != nullptr && rbnode->parent->lchild->color == RED)
					{
						rbnode->parent->lchild->color = BLACK;
						rightRotate(rbnode->parent);
					}
				}

				if (rbnode->parent->lchild == nullptr && rbnode->parent->rchild == nullptr && rbnode->parent->parent != nullptr)
				{
					// 'rbnode->parent->parent'ÎªRED
					rightRotate(rbnode->parent->parent);
				}

				free(rbnode);
			}
		}
	}

	// ÒÔrbnodeÎªÖÐÐÄ£¬½øÐÐ×óÐý
	void leftRotate(node *rbnode)
	{
		node *temp = rbnode->rchild;

		// ½øÐÐ×óÐý²Ù×÷
		rbnode->rchild = temp->lchild;
		if (temp->lchild != nullptr)
			temp->lchild->parent = rbnode;
		temp->lchild = rbnode;
		temp->parent = rbnode->parent;
		rbnode->parent = temp;

		// ¸üÐÂparent½Úµã
		if (root == rbnode)
		{
			root = temp;
			return;
		}
		if (temp->parent->lchild == rbnode)
			temp->parent->lchild = temp;
		else
			temp->parent->rchild = temp;
	}

	// ÒÔrbnodeÎªÖÐÐÄ£¬½øÐÐÓÒÐý²Ù×÷
	void rightRotate(node *rbnode)
	{
		node *temp = rbnode->lchild;

		// ½øÐÐÓÒÐý²Ù×÷
		rbnode->lchild = temp->rchild;
		if (temp->rchild != nullptr)
			temp->rchild->parent = rbnode;
		temp->rchild = rbnode;
		temp->parent = rbnode->parent;
		rbnode->parent = temp;

		// ¸üÐÂparent½Úµã
		if (root == rbnode)
		{
			root = temp;
			return;
		}
		if (temp->parent->lchild == rbnode)
			temp->parent->lchild = temp;
		else
			temp->parent->rchild = temp;
	}

	void PreOrder(node *&bt)
	{
		if (bt == nullptr)
		{
			return;
		}
		else
		{
			if (bt == root)
			{
				cout << bt->data << "\\" << bt->color;
			}
			else
			{
				cout << " " << bt->data << "\\" << bt->color;
			}
		}
		PreOrder(bt->lchild);
		PreOrder(bt->rchild);
	}
};

int main()
{
	int n;
	cin >> n;

	tree T;

	while (n--)
	{
		int a;
		cin >> a;
		T.insert(a);
	}
	T.PreOrder(T.root);

	cout << endl;
	return 0;
}

char *default_file_name = "sample.txt";
char *source_file_name = NULL;
char *target_file_name = NULL;
// write to standard output directly
// type == 1: write to standard output
// type == 2: append to target_file
void print_char(char c, int fd)
{
	write(fd, &c, 1);
}

void print(char *fmt)
{
	for (int i = 0; i < strlen(fmt); i++)
	{
		print_char(fmt[i], STDOUT_FILENO);
	}
}

// free memory allocated for string
void free_memory()
{
	if (source_file_name != NULL)
		free(source_file_name);
	if (target_file_name != NULL)
		free(target_file_name);
}

// After memory allocated, copy source string to destination string
char *copy_string(char *dst, char *src)
{
	if (dst != NULL)
		free(dst);
	dst = malloc(strlen(src) + 1);
	strcpy(dst, src);
	return dst;
}

int main(int argc, char *argv[])
{
	// set default word_count to 10
	int word_count = 10;
	// -a option flag
	int a_option_flag = 0;
	// copy default_file_name to source_file_name
	source_file_name = copy_string(source_file_name, default_file_name);

	// start handling the command line arguments
	if (argc >= 2)
	{
		//  the first argument must be the source file path argument if it is not started with '-'
		if (argv[1][0] != '-')
		{
			source_file_name = copy_string(source_file_name, argv[1]);
		}
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-a") == 0)
			{
				if (i + 1 < argc)
				{
					target_file_name = copy_string(target_file_name, argv[i + 1]);
					a_option_flag = 1;
					i++;
				}
				else
				{
					print("Invalid argument for -a option.\n");
					free_memory();
					return 1;
				}
			}
			else if (strcmp(argv[i], "-n") == 0)
			{
				if (i + 1 < argc)
				{
					word_count = atoi(argv[i + 1]);
					i++;
				}
				else
				{
					print("Invalid argument for -n option.\n");
					free_memory();
					return 1;
				}
			}
		}
	}

	// open source file
	int fd_src = open(source_file_name, O_RDONLY);
	int target_fd = STDOUT_FILENO;
	if (fd_src == -1)
	{
		print("Could not open file\n");
	}
	// start print or append specified number of words
	char ch;
	int count = 0, word_flag = 0; // word_flag indates that the ch is a part of the word or not
	// if -a option is specified, open the target_file
	if (a_option_flag == 1)
	{
		target_fd = open(target_file_name, O_WRONLY | O_CREAT);
	}
	while (read(fd_src, &ch, 1) == 1)
	{
		if (ch == ' ' || ch == '\t' || ch == '\n')
		{
			if (word_flag == 1)
			{
				count++;
				word_flag = 0;
				if (count < word_count)
					print_char(' ', target_fd);
			}
		}
		else
		{
			if (word_flag == 0)
				word_flag = 1;
			print_char(ch, target_fd);
		}
		if (count == word_count)
		{
			// if user input the -a option
			if (a_option_flag == 1)
			{
				print("Append successful\n");
			}
			break; // end
		}
	}

	// Over handling
	print("\n");
	// free memory
	free_memory();
	// Close the file
	close(fd_src);
	return 0;
}
