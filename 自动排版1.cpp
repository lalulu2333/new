#include <iostream>
#include <cstdio>
#include <queue>
#include <string.h> 
#include <vector>
#include <string>
#include <algorithm>
#include <sstream> 
using namespace std;
#define arraySize 200  //中心干部不超过100，每周班数最多50
#define inf 1000000000//无穷大
int capacity[arraySize][arraySize], flow[arraySize][arraySize], max_flow[arraySize], pre[arraySize];
//capacity存储点之间最大流量，flow存储点之间当前已经流过的流量 
//max_flow存储每次遍历过程中的值,pre记录查找过程中每个节点的前一节点，用于后续更新
int reflect[100];
//用于标记干部值哪班,初始值为0,代表还未分配值班时间
int period_info[60][10];
//每班由哪些人来值
int member_amount, period_amount, tmp, start_node, end_node;
//干部数量，班的数量，临时变量，超级源点，超级汇点


//干部对象,No-干部对应序号，name-姓名，period_amount可值班数，store_period存储可以值的班对应的序号，干部不超过100人
class  member
{
	public:
	int No;
	string name;
	int period_amount;
	vector <int> store_period;
	//为冒泡那里修改的拷贝函数 
	member()
	{
		
	}
	member (const  member & x)
	{
		No=x.No;
		name=x.name;
		period_amount=x.period_amount;
		store_period=x.store_period;
	}
}store_member[100];
//贪心阶段干部的排序，按照空余时间段少的先安排的策略排序
bool cmp(member a, member b)
{
	return a.period_amount < b.period_amount;
}

//初始化
void Initialize()
{
	//初始化干部值班对应关系
	memset(reflect, 0, sizeof(reflect));
	//初始化班存储的干部信息
	memset(period_info, 0, sizeof(period_info));
}

//核心算法，通过设置1个超级源点和1个超级汇点，并把从源点流出和流入源点的值设置为1，从而确保每一班都有一个人值
int Edmonds_Karp(int source, int target)//源点,汇点 
{
	//初始化 
	queue <int> store;
	int cur;
	//ans最大流 此处用于返回看能否保证每一班一个人
	int ans=0;
	//cur当前节点 
	//flow存储的是两个点之间的流量，用于后续判断干部和班之间的关系
	memset(flow, 0, sizeof(flow));
	while (true)//一直寻找增广路 
	{
		memset(max_flow, 0, sizeof(max_flow));
		memset(pre, 0, sizeof(pre));
		store.push(source);
		max_flow[source] = inf;
		while (!store.empty())
		{
			cur = store.front();
			store.pop();
			for (int next = source; next <= target; next++)
			{
				//max_flow[next]恰可以用于标记是否访问过，同时要保证两点之间还有剩余流量 
				//这个过程中，可能会出现多条可行路径，但因为汇点只有一个会被先到达的路径抢占，故每个过程只能找到一条
				if (!max_flow[next] && capacity[cur][next]>flow[cur][next])
				{
					store.push(next);
					//如果这两个点之间的值，比之前的最小值还小，则更新 
					max_flow[next] = min(max_flow[cur], capacity[cur][next] - flow[cur][next]);
					//记录前一个节点，用于后续更新 
					pre[next] = cur;
				}
			}
		}
		//说明已经找不到增广路了 
		if (max_flow[target] == 0)break;
		//更新操作 
		for (int u = target; u != source; u = pre[u])
		{
			flow[pre[u]][u] += max_flow[target];
			//反向边  
			flow[u][pre[u]] -= max_flow[target];
		}
		ans += max_flow[target];
	}
	return ans;
}

//最后排班信息显示
void display()
{
	//i控制每一班的循环 
	for (int i = 1; i <= period_amount; i++)
	{
		cout << "The people on duty in Period " << i << "are:   ";
		//j负责输出该班对应的几位干部 
		for (int j = 0; j < 10; j++)
		{
			//如果值非0，就代表还有干部，输出 
			if (period_info[i][j])
			    //period_info[i][j]存储的就是干部的序号，从而取出相应的干部的姓名 
				cout << "  "<<store_member[period_info[i][j]].name; 
			else break;
		}
		cout << endl;
	}
}

int main()
{
	//用于存储在EK算法之后，还没有被分配的干部
	vector <member> store_rest;
	Initialize();
	string s;//读入一行用的变量 
	int tmpstore;//下面临时存储一个干部可值班数的变量 
	//模拟系统数据获取，如若需要每月排班一次可以随机获得干部的输入顺序，即可获得不同的结果
	//建议最后可以添加一个功能,当一个人需要手动调整位置时，可以实现他在哪些班次可以移动 
	period_amount=15;
	//班数固定为49班,因为测试需要改成15班，因为自己没办法模拟那么大的测试数据，实际应该为49班 
	member_amount=0;
	//干部数通过计算有几次输入获得，初始值为0 
	cout << "请按序输入干部名字和可值班时段的序号\n";
	//member的下标必须从1开始，因为还要设置一个超级源点
     while(getline(cin,s))//此处需加一个终止输入符 ctrl+z 
	{
		member_amount++;
		istringstream myIn(s);
		//输入干部姓名
		myIn >> store_member[member_amount].name;
		tmpstore=0;//可以值班数先初始化为0 
		store_member[member_amount].No = member_amount;
		//输入干部可值班时间段对应序号
		while(myIn>>tmp)
		{
			tmpstore++;
			store_member[member_amount].store_period.push_back(tmp);
		}
		store_member[member_amount].period_amount=tmpstore;
	}
	//设置源点为0，1-member_amount为干部,
	//member_amount+1到member_amount+period_amount即为班次 
	start_node = 0;
	end_node = member_amount + period_amount + 1;
    //默认用序号代表班，从1到period_amount
	//建图
	//初始化，默认两点间有通路为1，无通路为0。
	memset(capacity, 0, sizeof(capacity));
	//超级源点为0，超级汇点为member_amount+period_amount+1
	//超级源点到每个干部间连一条权值为1的边，代表每个干部值1班
	for (int i = 1; i <= member_amount; i++)
	{
		capacity[0][i] = 1;
	}
	//每班到超级汇点之间连一条权值为1的边，从而先确保每一班保证有一个干部值
	for (int i = member_amount+1; i <= end_node-1; i++)
	{
		capacity[i][end_node] = 1;
	}
	for (int i = 1; i <= member_amount; i++)
	{
		for (int j = 0; j < store_member[i].period_amount; j++)
		{
			//干部和班之间权值置1，代表干部到班之间有一条路
			//store_member[i].store_period[j]存储的是第i个干部的第j班可以值哪个班加上member_amount的值就可以表示图中的点 
			capacity[i][store_member[i].store_period[j] + member_amount] = 1;
		}
	}
	//调用EK算法，先确保每一班有一人值。
	int check=Edmonds_Karp(start_node, end_node);
	//check用来检验是否能够保证每班至少1人,若不等于班数则代表不可以确保，不过这种情况几乎不可能发生 
	/*此处或许可以更加严密，如出现这种情况，就让某些干部值两班,出现这种情况，只要将源点到每个干部的权值设为2
	 保留每个班次到汇点的权值为1，细节部分需要稍作修改*/
	if (check != period_amount)
	{
		cout << "无法保障每班1人\n";
		return 0;
	}
	//给已经分配好的值班确认关系
	for (int i = member_amount + 1; i <= end_node - 1;i++)
	{
		for (int j = 1; j <= member_amount; j++)
		{
			//flow的值代表第j个干部是否值第(i-member_amount)个班,如果为1，则建立关系 
			if (flow[j][i] == 1)
			{
				reflect[j] = (i - member_amount);
				break;
			}
		}
	}
	//处理已经分配好的干部和对应班的关系 
	for (int i = 1; i <= member_amount; i++)
	{
		if(reflect[i])//如果第i个干部已经被分配了1班 
		tmp = reflect[i];
		else continue; //否则跳过 
		for (int j = 0; j < 10; j++)
		{
			//找到一个空值，把这个干部放进去 
			if (period_info[tmp][j] == 0)
			{
				period_info[tmp][j] = i;
				break;
			}
		}
	}
	//贪心阶段
	for (int i = 1; i <= member_amount; i++)
	{
		//还未被安排
		if (reflect[i] == 0)
		{
			store_rest.push_back(store_member[i]);
		}
	}
	//按照自定义的贪心策略排序,已修改为冒泡 
	member temp;
	for(int i=1;i<store_rest.size();i++)
       {
       	 for(int j=store_rest.size()-1;j>=i;j--)
       	 {
 	       if(store_rest[j].period_amount<store_rest[j-1].period_amount)
			{
				temp=store_rest[j];
				store_rest[j]=store_rest[j-1];
				store_rest[j-1]=temp;
			}  	
         }
       }
	int p, minn,cnt;
	//p用来记录该干部的哪个可值班中现有人数最少的那个班的序号
	//minn用来记录，该干部可值班中最小的现有人数 
	for (int i = 0; i < store_rest.size(); i++)//循环剩下的干部 
	{
		p = 0;
		minn = inf;
		//循环该干部的可值班，找出他可值班中哪个班现有人数最少，并将他安排到那个班次
		//同时注意及时更新班次信息，否则会在贪心选择时出错 
		for (int j = 0; j < store_rest[i].period_amount; j++)
		{
			cnt = 0;
			for (int k = 0; k < 10; k++)
			{
				//store_rest[i].store_period[j][k]剩下的第i个干部，他可值的第j个班的班的序号 
				if (period_info[store_rest[i].store_period[j]][k])
				{
					cnt++;
				}
				else break;
			}
			if (cnt < minn)
			{
				minn = cnt;
				//更新最小班的位置 
				p = store_rest[i].store_period[j];
			}
		}
		//更新 
		reflect[store_rest[i].No] = p;
        for (int k = 0; k < 10; k++)
			{
				if (period_info[p][k])continue;
				else 
				{
				  //更新班次信息 
				  period_info[p][k]=store_rest[i].No;
				  break;
				}
			} 
	}
	//显示 
	display();
	system("pause");
	return 0;
}
