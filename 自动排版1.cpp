#include <iostream>
#include <cstdio>
#include <queue>
#include <string.h> 
#include <vector>
#include <string>
#include <algorithm>
#include <sstream> 
using namespace std;
#define arraySize 200  //���ĸɲ�������100��ÿ�ܰ������50
#define inf 1000000000//�����
int capacity[arraySize][arraySize], flow[arraySize][arraySize], max_flow[arraySize], pre[arraySize];
//capacity�洢��֮�����������flow�洢��֮�䵱ǰ�Ѿ����������� 
//max_flow�洢ÿ�α��������е�ֵ,pre��¼���ҹ�����ÿ���ڵ��ǰһ�ڵ㣬���ں�������
int reflect[100];
//���ڱ�Ǹɲ�ֵ�İ�,��ʼֵΪ0,����δ����ֵ��ʱ��
int period_info[60][10];
//ÿ������Щ����ֵ
int member_amount, period_amount, tmp, start_node, end_node;
//�ɲ������������������ʱ����������Դ�㣬�������


//�ɲ�����,No-�ɲ���Ӧ��ţ�name-������period_amount��ֵ������store_period�洢����ֵ�İ��Ӧ����ţ��ɲ�������100��
class  member
{
	public:
	int No;
	string name;
	int period_amount;
	vector <int> store_period;
	//Ϊð�������޸ĵĿ������� 
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
//̰�Ľ׶θɲ������򣬰��տ���ʱ����ٵ��Ȱ��ŵĲ�������
bool cmp(member a, member b)
{
	return a.period_amount < b.period_amount;
}

//��ʼ��
void Initialize()
{
	//��ʼ���ɲ�ֵ���Ӧ��ϵ
	memset(reflect, 0, sizeof(reflect));
	//��ʼ����洢�ĸɲ���Ϣ
	memset(period_info, 0, sizeof(period_info));
}

//�����㷨��ͨ������1������Դ���1��������㣬���Ѵ�Դ������������Դ���ֵ����Ϊ1���Ӷ�ȷ��ÿһ�඼��һ����ֵ
int Edmonds_Karp(int source, int target)//Դ��,��� 
{
	//��ʼ�� 
	queue <int> store;
	int cur;
	//ans����� �˴����ڷ��ؿ��ܷ�֤ÿһ��һ����
	int ans=0;
	//cur��ǰ�ڵ� 
	//flow�洢����������֮������������ں����жϸɲ��Ͱ�֮��Ĺ�ϵ
	memset(flow, 0, sizeof(flow));
	while (true)//һֱѰ������· 
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
				//max_flow[next]ǡ�������ڱ���Ƿ���ʹ���ͬʱҪ��֤����֮�仹��ʣ������ 
				//��������У����ܻ���ֶ�������·��������Ϊ���ֻ��һ���ᱻ�ȵ����·����ռ����ÿ������ֻ���ҵ�һ��
				if (!max_flow[next] && capacity[cur][next]>flow[cur][next])
				{
					store.push(next);
					//�����������֮���ֵ����֮ǰ����Сֵ��С������� 
					max_flow[next] = min(max_flow[cur], capacity[cur][next] - flow[cur][next]);
					//��¼ǰһ���ڵ㣬���ں������� 
					pre[next] = cur;
				}
			}
		}
		//˵���Ѿ��Ҳ�������·�� 
		if (max_flow[target] == 0)break;
		//���²��� 
		for (int u = target; u != source; u = pre[u])
		{
			flow[pre[u]][u] += max_flow[target];
			//�����  
			flow[u][pre[u]] -= max_flow[target];
		}
		ans += max_flow[target];
	}
	return ans;
}

//����Ű���Ϣ��ʾ
void display()
{
	//i����ÿһ���ѭ�� 
	for (int i = 1; i <= period_amount; i++)
	{
		cout << "The people on duty in Period " << i << "are:   ";
		//j��������ð��Ӧ�ļ�λ�ɲ� 
		for (int j = 0; j < 10; j++)
		{
			//���ֵ��0���ʹ����иɲ������ 
			if (period_info[i][j])
			    //period_info[i][j]�洢�ľ��Ǹɲ�����ţ��Ӷ�ȡ����Ӧ�ĸɲ������� 
				cout << "  "<<store_member[period_info[i][j]].name; 
			else break;
		}
		cout << endl;
	}
}

int main()
{
	//���ڴ洢��EK�㷨֮�󣬻�û�б�����ĸɲ�
	vector <member> store_rest;
	Initialize();
	string s;//����һ���õı��� 
	int tmpstore;//������ʱ�洢һ���ɲ���ֵ�����ı��� 
	//ģ��ϵͳ���ݻ�ȡ��������Ҫÿ���Ű�һ�ο��������øɲ�������˳�򣬼��ɻ�ò�ͬ�Ľ��
	//�������������һ������,��һ������Ҫ�ֶ�����λ��ʱ������ʵ��������Щ��ο����ƶ� 
	period_amount=15;
	//�����̶�Ϊ49��,��Ϊ������Ҫ�ĳ�15�࣬��Ϊ�Լ�û�취ģ����ô��Ĳ������ݣ�ʵ��Ӧ��Ϊ49�� 
	member_amount=0;
	//�ɲ���ͨ�������м��������ã���ʼֵΪ0 
	cout << "�밴������ɲ����ֺͿ�ֵ��ʱ�ε����\n";
	//member���±�����1��ʼ����Ϊ��Ҫ����һ������Դ��
     while(getline(cin,s))//�˴����һ����ֹ����� ctrl+z 
	{
		member_amount++;
		istringstream myIn(s);
		//����ɲ�����
		myIn >> store_member[member_amount].name;
		tmpstore=0;//����ֵ�����ȳ�ʼ��Ϊ0 
		store_member[member_amount].No = member_amount;
		//����ɲ���ֵ��ʱ��ζ�Ӧ���
		while(myIn>>tmp)
		{
			tmpstore++;
			store_member[member_amount].store_period.push_back(tmp);
		}
		store_member[member_amount].period_amount=tmpstore;
	}
	//����Դ��Ϊ0��1-member_amountΪ�ɲ�,
	//member_amount+1��member_amount+period_amount��Ϊ��� 
	start_node = 0;
	end_node = member_amount + period_amount + 1;
    //Ĭ������Ŵ���࣬��1��period_amount
	//��ͼ
	//��ʼ����Ĭ���������ͨ·Ϊ1����ͨ·Ϊ0��
	memset(capacity, 0, sizeof(capacity));
	//����Դ��Ϊ0���������Ϊmember_amount+period_amount+1
	//����Դ�㵽ÿ���ɲ�����һ��ȨֵΪ1�ıߣ�����ÿ���ɲ�ֵ1��
	for (int i = 1; i <= member_amount; i++)
	{
		capacity[0][i] = 1;
	}
	//ÿ�ൽ�������֮����һ��ȨֵΪ1�ıߣ��Ӷ���ȷ��ÿһ�ౣ֤��һ���ɲ�ֵ
	for (int i = member_amount+1; i <= end_node-1; i++)
	{
		capacity[i][end_node] = 1;
	}
	for (int i = 1; i <= member_amount; i++)
	{
		for (int j = 0; j < store_member[i].period_amount; j++)
		{
			//�ɲ��Ͱ�֮��Ȩֵ��1������ɲ�����֮����һ��·
			//store_member[i].store_period[j]�洢���ǵ�i���ɲ��ĵ�j�����ֵ�ĸ������member_amount��ֵ�Ϳ��Ա�ʾͼ�еĵ� 
			capacity[i][store_member[i].store_period[j] + member_amount] = 1;
		}
	}
	//����EK�㷨����ȷ��ÿһ����һ��ֵ��
	int check=Edmonds_Karp(start_node, end_node);
	//check���������Ƿ��ܹ���֤ÿ������1��,�������ڰ������������ȷ������������������������ܷ��� 
	/*�˴�������Ը������ܣ�������������������ĳЩ�ɲ�ֵ����,�������������ֻҪ��Դ�㵽ÿ���ɲ���Ȩֵ��Ϊ2
	 ����ÿ����ε�����ȨֵΪ1��ϸ�ڲ�����Ҫ�����޸�*/
	if (check != period_amount)
	{
		cout << "�޷�����ÿ��1��\n";
		return 0;
	}
	//���Ѿ�����õ�ֵ��ȷ�Ϲ�ϵ
	for (int i = member_amount + 1; i <= end_node - 1;i++)
	{
		for (int j = 1; j <= member_amount; j++)
		{
			//flow��ֵ�����j���ɲ��Ƿ�ֵ��(i-member_amount)����,���Ϊ1��������ϵ 
			if (flow[j][i] == 1)
			{
				reflect[j] = (i - member_amount);
				break;
			}
		}
	}
	//�����Ѿ�����õĸɲ��Ͷ�Ӧ��Ĺ�ϵ 
	for (int i = 1; i <= member_amount; i++)
	{
		if(reflect[i])//�����i���ɲ��Ѿ���������1�� 
		tmp = reflect[i];
		else continue; //�������� 
		for (int j = 0; j < 10; j++)
		{
			//�ҵ�һ����ֵ��������ɲ��Ž�ȥ 
			if (period_info[tmp][j] == 0)
			{
				period_info[tmp][j] = i;
				break;
			}
		}
	}
	//̰�Ľ׶�
	for (int i = 1; i <= member_amount; i++)
	{
		//��δ������
		if (reflect[i] == 0)
		{
			store_rest.push_back(store_member[i]);
		}
	}
	//�����Զ����̰�Ĳ�������,���޸�Ϊð�� 
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
	//p������¼�øɲ����ĸ���ֵ���������������ٵ��Ǹ�������
	//minn������¼���øɲ���ֵ������С���������� 
	for (int i = 0; i < store_rest.size(); i++)//ѭ��ʣ�µĸɲ� 
	{
		p = 0;
		minn = inf;
		//ѭ���øɲ��Ŀ�ֵ�࣬�ҳ�����ֵ�����ĸ��������������٣����������ŵ��Ǹ����
		//ͬʱע�⼰ʱ���°����Ϣ���������̰��ѡ��ʱ���� 
		for (int j = 0; j < store_rest[i].period_amount; j++)
		{
			cnt = 0;
			for (int k = 0; k < 10; k++)
			{
				//store_rest[i].store_period[j][k]ʣ�µĵ�i���ɲ�������ֵ�ĵ�j����İ����� 
				if (period_info[store_rest[i].store_period[j]][k])
				{
					cnt++;
				}
				else break;
			}
			if (cnt < minn)
			{
				minn = cnt;
				//������С���λ�� 
				p = store_rest[i].store_period[j];
			}
		}
		//���� 
		reflect[store_rest[i].No] = p;
        for (int k = 0; k < 10; k++)
			{
				if (period_info[p][k])continue;
				else 
				{
				  //���°����Ϣ 
				  period_info[p][k]=store_rest[i].No;
				  break;
				}
			} 
	}
	//��ʾ 
	display();
	system("pause");
	return 0;
}
