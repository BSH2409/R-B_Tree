#include <iostream.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#define  line_y(m,x,c) (m*x+c)
#define  red 4
#define  black 8
#define  true 1
#define  false 0
float midx,midy;
class node
{
	public:
	int data,color;
	node *left,*right,*parent;
	node(int value)
	{
		data=value;
		color=red;
		left=right=parent=NULL;
	};
	node(){};
};
class cord
{
	public:
	float x,y;
	cord(float a=midx, float b=50)
	{
		x=a,y=b;
	};
};
class rb_tree
{
	public:
	node *rb_root;
    rb_tree(){
            rb_root=NULL;
            };
	void swap_values(int &,int &);
	int level_count(node*);
	cord node_cord(node*,node*,node*);
	void build_tree(node*,node*,float,float);
	void left_r(node*&,node*&);
	void right_r(node*&,node*&);
	void insert_fix(node*&,node*&);
	node *insertrb(node*,node*);
	void insert_node(int);
	void deleterb(node *,int);
	void delete_fix(node*&);
	void delete_node(int);
	void replace_node(node *, node *);
	node *search_node(node*,int);
	node *inorder_successor(node*);
};
class animation
{
    rb_tree tree;
    void gmatrix(const char*);
    void menu(int);
    int  op_sel();
    int ins_del();
    public:
    void start(int);
};


void rb_tree::swap_values(int &a, int &b)
{
      int temp= a; a=b; b=temp;

}
node* rb_tree::inorder_successor(node *ptr)
{
    while (ptr->left !=NULL)
		ptr=ptr->left;
    return ptr;
}

int rb_tree::level_count(node *ptr)
{
	int l_count=0,r_count=0;
	if(ptr==NULL) return 0;
	if(ptr->left!=NULL)
		{
			l_count+=1+level_count(ptr->left);
		}
	else
		l_count=0;
	if(ptr->right!=NULL)
		{
			r_count+=1+level_count(ptr->right);
		}
	else r_count=0;
	return l_count>=r_count? l_count:r_count;
}
cord rb_tree::node_cord(node *root,node *cur,node *cur_p=NULL)
{
	static cord xy;
	if(root==NULL||(cur==NULL&&cur_p==NULL)) return cord(-1,-1);
	if(cur==NULL)
		{    cord  p=node_cord(rb_root,cur_p);
			if(cur_p->left==NULL)
				xy.x=p.x-40,
				xy.y=p.y+60;
			if(cur_p->right==NULL)
				xy.x=p.x+40,
				xy.y=p.y+60;
			cord t=xy;xy=cord();
			return t ;
		}
	if(root==cur)
		{
			cord t=xy;xy=cord();
			return t ;
		}
	if(root->data>cur->data)
		{
			xy.x-=40+20*(level_count(root->left)),xy.y+=60;
			return node_cord(root->left,cur);
		}
	else if(root->data<cur->data)
	{
		xy.x+=40+20*(level_count(root->right)),xy.y+=60;
		return node_cord(root->right,cur);
	}
	return cord(-1,-1);
}

void rb_tree::build_tree(node *root,node *pause,float x=midx, float y=50)
{
	char ch[3];
	int i=0,j=0;
	float dx,dy;
	outtextxy(midx-100,10,"RB TREE VISUALIZATION");
	rectangle(30,20,getmaxx()-30,50+60*(level_count(rb_root)+1));
	setbkcolor(LIGHTBLUE);
		if(root==pause||root==NULL) return;
	setcolor(root->color);
	circle(x,y,15);outtextxy(x-2,y-2,itoa(root->data,ch,10));
	setcolor(WHITE);
		if(root->left==NULL&&root->right==NULL) return;
		if(root->left!=pause)
			{
				 if(level_count(root->left)>0) i=20*level_count(root->left);
				 dx=20*cos(atan(60.0/(40.0+i))),dy=20*sin(atan(60.0/(40.0+i)));
				 line(x-dx,y+dy,x-40-i+dx,y+60-dy);
				 build_tree(root->left,pause,x-40-i,y+60);
			}
		if(root->right!=pause)
			{
				if(level_count(root->right)>0) j=20*level_count(root->right);
				dx=20*cos(atan(60.0/(40.0+j))),dy=20*sin(atan(60.0/(40.0+j)));
				line(x+dx,y+dy,x+40+j-dx,y+60-dy);
				build_tree(root->right,pause,x+40+j,y+60);
			}

}
void rb_tree::left_r(node *&root,node *&x)
{
	char ch[3];
	cord Xi=node_cord(root,x);
	cord Xj=node_cord(root,x->left,x);
	cord Yi=node_cord(root,x->right);
	float mx=(Xi.y-Xj.y)/(Xi.x-Xj.x),cx=Xj.y-mx*Xj.x;
	float my=(Xi.y-Yi.y)/(Xi.x-Yi.x),cy=Yi.y-my*Yi.x;
	cord a=node_cord(root,x->right->left),
	     b=node_cord(root,x->right->right);
       for(float x1=Xi.x,x2=Yi.x;x1>=Xj.x||x2>=Xi.x;)
	{
		cleardevice();
		build_tree(root,x);
		build_tree(x->left,NULL,Xj.x-Xi.x+x1,Xj.y+Xi.x-x1);
		build_tree(x->right->left,NULL,a.x-Xi.x+x1,a.y);
		build_tree(x->right->right,NULL,b.x-Xi.x+x1,b.y-Xi.x+x1);
		if(x1>=Xj.x)
		{    setcolor(x->color);
		     outtextxy(x1-2,line_y(mx,x1,cx)-2,itoa(x->data,ch,10));
			circle(x1,line_y(mx,x1,cx),15);
				 x1--;
		}
		else
		{      setcolor(x->color);
			circle(x1,line_y(mx,x1,cx),15);
			 outtextxy(x1-2,line_y(mx,x1,cx)-2,itoa(x->data,ch,10));
		}
		if(x2>=Xi.x)
		{    setcolor(x->right->color);
		     outtextxy(x2-2,line_y(my,x2,cy)-2,itoa(x->right->data,ch,10));
			circle(x2,line_y(my,x2,cy),15);
				 x2--;
		}
		else
		{     setcolor(x->right->color);
			circle(x2,line_y(my,x2,cy),15);
			 outtextxy(x2-2,line_y(my,x2,cy)-2,itoa(x->right->data,ch,10));
		}
	delay(60);  	setcolor(WHITE);
	}

	// */
	node * y= x->right;
	x->right=y->left;
	if(x->right!=NULL)	x->right->parent=x;
	y->parent=x->parent;
	if(x->parent==NULL)	rb_root=y;
	else
	if(x==x->parent->left)  x->parent->left=y;
	else			x->parent->right=y;
	y->left=x;
	x->parent=y;
	cleardevice();
}
void rb_tree::right_r(node *&root,node *&x)
{
	char ch[3];
	cord Xi=node_cord(rb_root,x);
	cord Xj=node_cord(rb_root,x->right,x);
	cord Yi=node_cord(rb_root,x->left,x);
	float mx=(Xi.y-Xj.y)/(Xi.x-Xj.x),cx=Xj.y-mx*Xj.x;
	float my=(Xi.y-Yi.y)/(Xi.x-Yi.x),cy=Yi.y-my*Yi.x;
	cord a=node_cord(rb_root,x->left->left,x->left),
	     b=node_cord(rb_root,x->left->right,x->left);
	for(float x1=Xi.x,x2=Yi.x;x1<=Xj.x||x2<=Xi.x;)
	{
		cleardevice();
		build_tree(rb_root,x);
		build_tree(x->right,NULL,Xj.x-Xi.x+x1,Xj.y-Xi.x+x1);
		build_tree(x->left->left,NULL,a.x-Xi.x+x1,a.y+Xi.x-x1);
		build_tree(x->left->right,NULL,b.x-Xi.x+x1,b.y);

		if(x1<=Xj.x)
		{
		    outtextxy(x1-2,line_y(mx,x1,cx)-2,itoa(x->data,ch,10));
			setcolor(x->color);
			circle(x1,line_y(mx,x1,cx),15);
			x1++;
		}
		else
		{
	    setcolor(x->color);
			circle(x1,line_y(mx,x1,cx),15);
			outtextxy(x1-2,line_y(mx,x1,cx)-2,itoa(x->data,ch,10));
		}
		if(x2<=Xi.x)
		{
		    setcolor(x->left->color);
		    outtextxy(x2-2,line_y(my,x2,cy)-2,itoa(x->left->data,ch,10));
			circle(x2,line_y(my,x2,cy),15);
			x2++;
		}
		else
		{
			setcolor(x->left->color);
			circle(x2,line_y(my,x2,cy),15);
	    outtextxy(x2-2,line_y(my,x2,cy)-2,itoa(x->left->data,ch,10));
		}
		delay(60);
	}
	// */
	node *y=x->left;
	x->left=y->right;
	if(x->left!=NULL) x->left->parent=x;
	y->parent=x->parent;
	if(x->parent==NULL) root=y;
	else if(x==x->parent->left) x->parent->left=y;
	else x->parent->right=y;
	y->right=x;
	x->parent=y;
	cleardevice();
}
void rb_tree::insert_fix(node *&root,node *&pt)
{
	node *parent_pt = NULL;
	node *grand_parent_pt = NULL;
	while ((pt != root) &&
		(pt->color != black) &&
		 (pt->parent->color == red))
	{
		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		if (parent_pt == grand_parent_pt->left)
		{

			node *uncle_pt = grand_parent_pt->right;

			if (uncle_pt!=NULL&&
				uncle_pt->color==red)
			{
				grand_parent_pt->color = red;
				parent_pt->color = black;
				uncle_pt->color = black;
				pt = grand_parent_pt;
			}

			else
			{
				if (pt == parent_pt->right)
				{
				left_r(root,parent_pt);
				pt = parent_pt;
				parent_pt = pt->parent;
				}

			right_r(root,grand_parent_pt);
			swap_values(parent_pt->color,grand_parent_pt->color);
			pt = parent_pt;
			}
		}

		else
		{
			node *uncle_pt = grand_parent_pt->left;


			if((uncle_pt !=NULL)&&
				 (uncle_pt->color==red))
			{
				grand_parent_pt->color = red;
				parent_pt->color = black;
				uncle_pt->color = black;
				pt = grand_parent_pt;
			}
			else
			{
				if(pt == parent_pt->left)
				{
				    right_r(root,parent_pt);
					pt = parent_pt;
					  parent_pt = pt->parent;
				}

			left_r(root,grand_parent_pt);
			swap_values(parent_pt->color, grand_parent_pt->color);
			pt = parent_pt;
			}
		}
	    }

	root->color = black;
}
void rb_tree:: delete_fix(node *&ptr)
{
    node *temp;
    while (ptr != rb_root && (ptr->color ==black||ptr==NULL)) {
      if (ptr == ptr->parent->left) {
	temp = ptr->parent->right;
	if (temp->color == red) {
	  temp->color = black;
	  ptr->parent->color = red;
	  left_r(rb_root,ptr->parent);
	  temp = ptr->parent->right;
	}

	if ((temp->left->color == black||temp->left==NULL) &&(temp->right==NULL||temp->right->color == black)) {
	  temp->color = red;
	  ptr = ptr->parent;
	} else {
	  if (temp->right==NULL||temp->right->color ==black) {
	    temp->left->color = black;
	    temp->color = red;
	    right_r(rb_root,temp);
	    temp = ptr->parent->right;
	  }

	  temp->color = ptr->parent->color;
	  ptr->parent->color = black;
	  temp->right->color = black;
	  left_r(rb_root,ptr->parent);
	  ptr = rb_root;
	}
      } else {
	temp = ptr->parent->left;
	if (temp->color == red) {
	  temp->color = black;
	  ptr->parent->color = red;
	  right_r(rb_root,ptr->parent);
	  temp = ptr->parent->left;
	}

	if ((temp->right->color == black||temp->right==NULL) &&(temp->right==NULL||temp->right->color == black)){
	  temp->color = red;
	  ptr = ptr->parent;
	} else {
	  if (temp->left->color == black||temp->left==NULL) {
	    temp->right->color = black;
	    temp->color = red;
	    left_r(rb_root,temp);
	    temp = ptr->parent->left;
	  }

	  temp->color = ptr->parent->color;
	  ptr->parent->color = black;
	  temp->left->color = black;
	  right_r(rb_root,ptr->parent);
	  ptr = rb_root;
	}
      }
    }
    ptr->color = black;
}
node* rb_tree::insertrb(node*root,node *temp)
{
	if(root==NULL) return temp;
	cord xy=node_cord(rb_root,root);
	char ch[3];
	build_tree(rb_root,NULL);
	circle(50,50,15),outtextxy(48,48,itoa(temp->data,ch,10));
	outtextxy(xy.x-2,xy.y-2,itoa(root->data,ch,10));
		setcolor(BLUE);
		circle(xy.x,xy.y,15);delay(400);
		setcolor(root->color);
		circle(xy.x,xy.y,15);
	delay(400);
	cleardevice();
	if(temp->data>root->data)
	{
		root->right=insertrb(root->right,temp);
		root->right->parent=root;
	}
	else
	{
		root->left=insertrb(root->left,temp);
		root->left->parent=root;
	}
	return root;
}
void rb_tree::insert_node(int data)
{
	if(search_node(rb_root,data)!=NULL)
	{
	 outtextxy(midx-70,midy+170,"Value Already Exists");
	 delay(1000);return;
	}
	node *temp=new node(data);
	temp->left=NULL;
	temp->right=NULL;
	rb_root=insertrb(rb_root,temp);
	insert_fix(rb_root,temp);
	build_tree(rb_root,NULL);
	delay(500);

}
void rb_tree::replace_node(node *u, node *v) {
    if (u->parent == NULL) {
      rb_root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }
void rb_tree::deleterb(node *root,int data)
{
    node *ptr = NULL;
    node *x, *y;
    ptr=search_node(root,data);
    if (ptr == NULL) {
      outtextxy(midx-38,midy+170,"Not Found");
      delay(1000);
      return;
    }

    y = ptr;
    int y_color = y->color;
    if (ptr->left == NULL) {
      x = ptr->right;
      replace_node(ptr, ptr->right);
    } else if (ptr->right ==NULL) {
      x = ptr->left;
      replace_node(ptr, ptr->left);
    } else {
      y = inorder_successor(ptr->right);
      y_color = y->color;
      if(y==NULL)y_color=-1;
      x = y->right;
      if (y->parent == ptr) {
	x->parent = y;
      } else {
	replace_node(y, y->right);
	y->right = ptr->right;
	y->right->parent = y;
      }

      replace_node(ptr, y);
      y->left = ptr->left;
      y->left->parent = y;
      y->color = ptr->color;
    }
    delete ptr;

     if(y_color==black||y_color==-1) delete_fix(x);

}
void rb_tree::delete_node(int data)
{
   deleterb(rb_root,data);
   cleardevice();
   build_tree(rb_root,NULL);
   delay(500);
}
node* rb_tree::search_node(node *root,int data)
{
	while (root != NULL) {
	if (data<root->data) {
		if (root->left == NULL)
		break;
		else
		root = root->left;
	} else if (data== root->data) {
		return root;
	} else {
		if (root->right == NULL)
		break;
		else
		root = root->right;
	}
	}

	return NULL;
}

const char* shuffle_str(char ch[])
{
   char str[100]; strcpy(str,ch);
   const int l=strlen(str);
   static int valve[100]={0};
   srand(time(NULL));
   for(int i=0;i<l;i+=2)
	{
	if(valve[i]) str[i]=ch[rand()%l];
	else str[i]=' ';
	}
	for(i=0;i!=15;i++)
	{
	int x=rand()%100;
	valve[x]=!valve[x];
	}
   return str;
}
void animation::gmatrix(const char * ch)
{
const int y_limit=getmaxy();
char str[]="01 0   1 0 0 1010      0 10 1 01 0100        1 01 1 001010";
char pattern[23][31];
for(int i=0;i<23;i++)
{
	strcpy(pattern[i],shuffle_str(str));
}
settextstyle(0,0,1);
setfillstyle(1,BLACK);
for(i=0;i<y_limit;i+=15)
{
for(int j=0;j<23;j++)
{
	int k=(i+j*20)%getmaxy();
	if((k>10&&k<midy-50)||(k>midy+50&&k<getmaxy()-10))
	{int arr[]={0,k, getmaxx(),k, getmaxx(),k+20,0,k+20, 0,k};
	setcolor(BLACK);
	fillpoly(4,arr);
	setcolor(LIGHTGREEN);
	outtextxy(0,k,pattern[j]);
	}
//	settextstyle(2,0,1);
	setcolor(WHITE);
	outtextxy(midx-130,midy,ch);
	settextstyle(0,0,0);
}
}
}
void animation ::menu(int a)
{       cleardevice();
	setfillstyle(3,DARKGRAY);
	setbkcolor(LIGHTBLUE);
	{int arr[]={midx-250,midy+200, midx-150, midy+200,
			midx-150,midy+220, midx-250,midy+220
				,midx-250,midy+200};
			fillpoly(4,arr);outtextxy(midx-225,midy+207,"INSERT");
			}
	{int arr[]={midx+250,midy+200, midx+150, midy+200,
			midx+150,midy+220, midx+250,midy+220
				,midx+250,midy+200};
			fillpoly(4,arr);outtextxy(midx+175,midy+207,"DELETE");
			}
	setfillstyle(1,WHITE);
	if(a==0){int arr[]={midx-250,midy+200, midx-150, midy+200,
			midx-150,midy+220, midx-250,midy+220
				,midx-250,midy+200};
			fillpoly(4,arr);
			setcolor(BLACK);
			outtextxy(midx-225,midy+207,"INSERT");
			}
	else if(a==1){int arr[]={midx+250,midy+200, midx+150, midy+200,
			midx+150,midy+220, midx+250,midy+220
				,midx+250,midy+200};
			fillpoly(4,arr);
			setcolor(BLACK);
			outtextxy(midx+175,midy+207,"DELETE");
			}

	setfillstyle(0,0);
	setcolor(WHITE);
	tree.build_tree(tree.rb_root,NULL);

}
int animation::ins_del()
{
int res=-1;
menu(4);
while(1)
{

   switch(getch())
   {
	case 72:
	case 75: menu(0) ;res=0 ;break;
	case 80:
	case 77: menu(1) ;res=1 ;break;
	case 13: return res;
	case 27: return -1;
   };

}
}
int sti(char ch[])
{
	int l= strlen(ch),num=0;
	for(int i=0;i<l;i++)
	   { num+=ch[i]-48; num*=10;}
	return num/10;
}
int animation::op_sel()
{
	int flag=ins_del();
	if(flag==-1) return -1;
	else
	{
	  int arr[]={midx-50,midy+200, midx+50, midy+200,
			midx+50,midy+220, midx-50,midy+220
				,midx-50,midy+200};
			fillpoly(4,arr);
			setcolor(WHITE);
			outtextxy(midx-43,midy+188,"ENTER VALUE");


	  char ch[5];
	  int i=0,on=1;
	  while(on)
	  {
	  char a=getch(); cout<<"\b";
	  switch(a)
	  {
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:ch[i++]=a;ch[i]='\0'; break;
		case 13:on=0;                 break;
		case  8:ch[--i]='\0';break;
		default:cout<<"\b";
	  }
	  {int arr[]={midx-50,midy+200, midx+50, midy+200,
			midx+50,midy+220, midx-50,midy+220
				,midx-50,midy+200};
			setfillstyle(1,BLACK);
			fillpoly(4,arr);}
	  outtextxy(midx-20,midy+205,ch);
	  }
	  if(flag==0){tree.insert_node(sti(ch)); return 1;}
		else if(flag==1){tree.delete_node(sti(ch));return 1;}


	}
return -1;
}
void animation:: start(int flag)
{
	cleardevice();
    if(flag==true)
    {
		 gmatrix("WELCOME TO RB TREE VISUALISATION");
	    while(1)
		{
		if(op_sel()==-1){flag=false; break;}
		};
	    cleardevice();

	    tree.build_tree(tree.rb_root,NULL);
	    delay(1000);
    }
    else if(flag==false){setbkcolor(BLACK); gmatrix("            THANK YOU           ");}
}
void main()
{
 int gdriver = DETECT, gmode, errorcode;
 initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI\\");
 midx=getmaxx()/2,midy=getmaxy()/2;
    animation display;
    display.start(true);
    display.start(false);

 closegraph();
}
