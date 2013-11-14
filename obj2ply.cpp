#include "StdAfx.h"
#include "obj2ply.h"
#include "Lib3D/Base3d.h"

int myMain::f_num=0;
int myMain::p_num=0;
myVector* myMain::p=NULL; 
face* myMain::f=NULL;
ballPoint* myMain::queue=NULL;
geodist** myMain::distMatrix=NULL;
bool* myMain::Visible=NULL;
CMesh3d* myMain::pMeshDebug=new CMesh3d;
int myMain::selectedShowFace=836;
int myMain::USEJUNCTIONDETECTION=1;
int myMain::USEINTEGRAL=1;
int myMain::USEBOUNDARY=1;
int myMain::DEBUGMODE=0;


// Add mesh


myMain::myMain()
{
}

myMain::~myMain()
{

}

void myMain::output_ply()
{

	const char* outfilenamechar=filename_ply.c_str();

	ofstream fout(outfilenamechar);

	fout<<"ply"<<endl;
	fout<<"format ascii 1.0"<<endl;
	fout<<"comment authorized by Guanyu He"<<endl;
	
	fout<<"element vertex "<<p_num<<endl;
	fout<<"property double x"<<endl;
	fout<<"property double y"<<endl;
	fout<<"property double z"<<endl;

	if(COLORFACE==0)
	{
		fout<<"property uchar red"<<endl;
		fout<<"property uchar green"<<endl;
		fout<<"property uchar blue"<<endl;
	}



	fout<<"element face "<<f_num<<endl;
	fout<<"property list uchar int vertex_indices"<<endl;
	if(COLORFACE==1)
	{
		fout<<"property uchar red"<<endl;
		fout<<"property uchar green"<<endl;
		fout<<"property uchar blue"<<endl;
	}

	fout<<"end_header"<<endl;

	for(int i=0;i<p_num;i++)
	{
		if(COLORFACE==1)fout<<p[i].x<<" "<<p[i].y<<" "<<p[i].z<<endl;
		else fout<<p[i].x<<" "<<p[i].y<<" "<<p[i].z<<" "<<(int)pColor[i].r<<" "<<(int)pColor[i].g<<" "<<(int)pColor[i].b<<endl;
	}
	for(int i=0;i<f_num;i++)
	{	
		if(COLORFACE==1) fout<<"3 "<<f[i].p1index<<" "<<f[i].p2index<<" "<<f[i].p3index<<" "<<(int)fColor[i].r<<" "<<(int)fColor[i].g<<" "<<(int)fColor[i].b<<endl;
		else fout<<"3 "<<f[i].p1index<<" "<<f[i].p2index<<" "<<f[i].p3index<<endl;
	}
	fout.close();
}

void myMain::output_wrl()
{
	const char* outfilenamechar=filename_wrl.c_str();

	ofstream fout(outfilenamechar);
	fout<<"#VRML V2.0 utf8"<<endl<<endl;
	fout<<"# Produced By obj2wrl.exe"<<endl;
	fout<<"# Copyright Guanyu He, CG&CAD, School of Software, Tsinghua University"<<endl<<endl;
	fout<<"DEF Guanyu_"<<filename_noext<<" Transform {"<<endl;
	fout<<"  translation 0 0 0"<<endl;
	fout<<"  children ["<<endl;
	fout<<"    Shape {"<<endl;
	fout<<"      appearance Appearance {"<<endl;
	fout<<"        material Material {"<<endl;
	fout<<"          diffuseColor 0.8784 0.3412 0.5608"<<endl;
	fout<<"        }"<<endl;
	fout<<"      }"<<endl;
	fout<<"      geometry DEF Guanyu_"<<filename_noext<<"-FACES IndexedFaceSet {"<<endl;
	fout<<"        ccw TRUE"<<endl;
	fout<<"        solid TRUE"<<endl;
	fout<<"        coord DEF Guanyu_"<<filename_noext<<"-COORD Coordinate { point ["<<endl;
	fout<<"          ";

	int precision=5;
	fout.unsetf(ios::scientific);
	fout.setf(ios::fixed);
	fout.unsetf(ios::showpoint);
	fout.precision(6);
	
	for(int i=0;i<p_num;i++)
	{
		fout<<p[i].x<<" "<<p[i].y<<" "<<p[i].z;
		if(i<p_num-1)fout<<", ";
		if(i%3==2) fout<<endl<<"          ";
	}
	fout<<"]"<<endl<<"        }"<<endl;
	fout<<"        coordIndex ["<<endl;

	fout<<"          ";
	for(int i=0;i<f_num;i++)
	{
		fout<<f[i].p1index<<", "<<f[i].p2index<<", "<<f[i].p3index<<", -1";
		if(i<f_num-1) fout<<", ";
		if(i%4==3)fout<<endl<<"          ";
	}
	fout<<"]"<<endl;
	fout<<"        }"<<endl;
	fout<<"    }"<<endl;
	fout<<"  ]"<<endl;
	fout<<"}"<<endl;

	fout.close();
}

void myMain::conv_filename()
{

	int i,l;

	l=filename.length();
	i=l-1;
	while(filename[i]!='.') i--;
	filename_noext=filename.substr(0,i);
	filename_wrl=filename_noext+".wrl";
	filename_ply=filename_noext+".ply";
	filename=filename;
	filename_viv=filename_noext+".viv";
}


void myMain::readfile()
{

	p=(myVector*)malloc(sizeof(myVector)*20000);
	f=(face*)malloc(sizeof(face)*20000);

	p_num=0;
	f_num=0;

	const char* filenamechar=filename.c_str();

	ifstream fin(filenamechar);
	if(!fin)
	{
		string errormessage="Error occurs when opening file \""+filename+"\"";
		AfxMessageBox(errormessage.c_str());
		return;
	}

	char firstc,tmp;
	char* line=(char*)malloc(sizeof(char)*200);
	double px,py,pz;
	int p1,p2,p3;
	while(!fin.eof())
	{
		fin.get(firstc);
		if(firstc=='v')
		{
			fin.get(tmp);
			fin>>p[p_num].x>>p[p_num].y>>p[p_num].z;
			fin.getline(line,100);
			p_num++;
			firstc=' ';
		}
		else if(firstc=='f')
		{
			fin.get(tmp);
			fin>>f[f_num].p1index>>f[f_num].p2index>>f[f_num].p3index;

			f[f_num].p1index--;f[f_num].p2index--;f[f_num].p3index--;
			f[f_num].p1=p[f[f_num].p1index];
			f[f_num].p2=p[f[f_num].p2index];
			f[f_num].p3=p[f[f_num].p3index];

			f[f_num].getCenter();
			f[f_num].getNormal();
			f[f_num].getArea();

			fin.getline(line,100);
			f_num++;
			firstc=' ';
		}
		else if(firstc=='\n')
		{
			firstc=' ';
		}
		else
		{
			fin.getline(line,100);
			firstc=' ';
		}
	}
	
	fin.close();
}

void myMain::computeVIV(bool refreshMeshDebug)
{



	pMeshDebug->m_ArrayLine.RemoveAll();
	pMeshDebug->m_ArrayVertex.RemoveAll();
	pMeshDebug->m_ArrayFace.RemoveAll();

	const char* outfilename=filename_viv.c_str();
	char* line=(char*)malloc(sizeof(char)*100);

	if(refreshMeshDebug==false)
	{
		p_face=(int*)malloc(sizeof(int)*p_num);
		fColor=(myColor*)malloc(sizeof(myColor)*f_num);
		pColor=(myColor*)malloc(sizeof(myColor)*p_num);
		for(int i=0;i<p_num;i++)
		{
			pColor[i].tag=0;
			p_face[i]=0;
		}
		ifstream _file(outfilename);
		if(_file)
		{
			int check=IDNO;
			//check=MessageBox(0,"VIV file detected, Refresh it(yes) or Use it(no)?","VIV file exists",MB_YESNO);

			if(check==IDNO)
			{
				minf=MAXdouble;maxf=0;

				for(int i=0;i<f_num;i++)
				{
					
					_file>>fColor[i].tag;
					_file.getline(line,100);

					if(fColor[i].tag>maxf)maxf=fColor[i].tag;
					if(fColor[i].tag<minf)minf=fColor[i].tag;

					pColor[f[i].p1index].tag+=fColor[i].tag;p_face[f[i].p1index]++;
					pColor[f[i].p2index].tag+=fColor[i].tag;p_face[f[i].p2index]++;
					pColor[f[i].p3index].tag+=fColor[i].tag;p_face[f[i].p3index]++;
					
				}
				_file.close();	
				return;
			}
			_file.close();
		}

		
		for(int i=0;i<p_num;i++)p_face[i]=0;

		queue=(ballPoint*)malloc(sizeof(ballPoint)*SEGMENTNUM*100);
		Visible=(bool*)malloc(sizeof(bool)*f_num);

		for(int i=0;i<f_num;i++) Visible[i]=false;
	}
	ofstream outf;
	if(DEBUGMODE==NODEBUG)	outf.open(outfilename);
	if(queue==NULL)queue=(ballPoint*)malloc(sizeof(ballPoint)*SEGMENTNUM*100);
	if(Visible==NULL)Visible=(bool*)malloc(sizeof(bool)*f_num);
			
	for(int i=0;i<f_num;i++)
	{

		if(DEBUGMODE==CHECKVISIBILITY) if(i!=selectedShowFace) continue;

		double ans=f[i].visibleVolume(i,DEBUGMODE);

		ans/=100000;
		if(DEBUGMODE==CHECKVISIBILITY)
		{
			fColor[i].r=255;fColor[i].g=0;fColor[i].b=0;
			for(int j=0;j<f_num;j++) if (i!=j)
			{
				fColor[j].r=0;
				fColor[j].g=Visible[j]?255:0;
				fColor[j].b=Visible[j]?0:255;
			}
			avgf=125;
			minf=0;
			maxf=255;
			break;
		}

		avgf+=ans;
		if(minf>ans) minf=ans;
		if(maxf<ans)maxf=ans;

		fColor[i].tag=ans;
		pColor[f[i].p1index].tag+=ans;p_face[f[i].p1index]++;
		pColor[f[i].p2index].tag+=ans;p_face[f[i].p2index]++;
		pColor[f[i].p3index].tag+=ans;p_face[f[i].p3index]++;

		outf<<ans<<endl;
	}
	if(DEBUGMODE==NODEBUG)outf.close();
	
}

void myMain::computeResult()
{



	c.BuildRainbow();

	maxi=0;mini=MAXdouble;
	if(COLORFACE==0)
	{
		for(int i=0;i<p_num;i++)
		{
			pColor[i].tag/=p_face[i];

			if(mini>pColor[i].tag)mini=pColor[i].tag;
			if(maxi<pColor[i].tag)maxi=pColor[i].tag;
		}

		for(int i=0;i<p_num;i++)
		{
			pColor[i].tag-=mini;
			pColor[i].tag/=(maxi-mini);
			pColor[i].tag*=255;

			if(USEBOUNDARY==1)
				if(USEJUNCTIONDETECTION==1)
					if(pColor[i].tag>p_color_boundary)pColor[i].tag=195;else pColor[i].tag=0;

			if(USERAINBOW==1)
			{
				pColor[i].r=c.Red((int)(pColor[i].tag));
				pColor[i].g=c.Green((int)pColor[i].tag);
				pColor[i].b=c.Blue((int)pColor[i].tag);
			}
			else
			{
				pColor[i].r=pColor[i].tag;pColor[i].g=pColor[i].tag;pColor[i].b=pColor[i].tag;
			}
		}
	}
	else if(COLORFACE==1)
	{

		for(int i=0;i<f_num;i++)
		{
			fColor[i].tag-=minf;
			fColor[i].tag/=(maxf-minf);
			fColor[i].tag*=255;



			if(USERAINBOW==1)
			{
				fColor[i].r=c.Red(fColor[i].tag);
				fColor[i].g=c.Green(fColor[i].tag);
				fColor[i].b=c.Blue(fColor[i].tag);
			}
			else
			{
				fColor[i].r=fColor[i].tag;fColor[i].g=fColor[i].tag;fColor[i].b=fColor[i].tag;
			}

		}

	}
	free(queue);
}

void myMain::junction_detection()
{
	int i;
	edge** nearby=(edge**)malloc(sizeof(edge*)*p_num);
	distMatrix=(geodist**)malloc(sizeof(geodist*)*f_num);
	int* geodesicNum=(int*)malloc(sizeof(int)*f_num);
	for(i=0;i<f_num;i++)
	{
		distMatrix[i]=(geodist*)malloc(sizeof(geodist)*150);
		geodesicNum[i]=0;
	}

	int* nearbynum=(int*)malloc(sizeof(int)*p_num);
	for(i=0;i<p_num;i++)
	{
		nearby[i]=(edge*)malloc(sizeof(edge)*30);
		nearbynum[i]=0;
	}

	//接下来的一大段程序都是用来扩展测地矩阵的。用的是类似dijkstra的算法，但是设定扩展次数在5次，也就是说5步之内能访问到的面就是
	//邻居面。geodesicNum[i]表示第i个面有多少个邻居面。distmatrix[i][j]表示的是第i个面的第j个邻居面的测地距离。
	//再次强调，这里的j表示的是i的第j个邻居面，而不是模型的第j个面

	//这一部分程序用来计算初始的邻居面（直接相邻）
	int temp;bool flag;
	int x[3];
	for(i=0;i<f_num;i++)
	{
		x[0]=f[i].p1index;x[1]=f[i].p2index;x[2]=f[i].p3index;
		if(x[0]>x[1]){ temp=x[0];x[0]=x[1];x[1]=temp;}
		if(x[0]>x[2]){ temp=x[0];x[0]=x[2];x[2]=temp;}
		if(x[1]>x[2]) {temp=x[1];x[1]=x[2];x[2]=temp;}

		for(int nowp=0;nowp<=2;nowp++)
		{
			int endp=nowp+1;endp%=3;
			int now=nowp;if(nowp==2){now=0;endp=2;}				//get an edge
			flag=false;
			for(int j=0;j<nearbynum[x[now]];j++) if(!flag)
			{
				if(nearby[x[now]][j].toVertexIndex==x[endp])
				{
					nearby[x[now]][j].f2Index=i;
					myVector middle=p[x[now]].add(p[x[endp]]).productReal(0.5);
					int startface=nearby[x[now]][j].f1Index,endface=i;
					if(startface<0 || startface>=f_num || endface<0 || endface>=f_num || startface==endface)
						AfxMessageBox("Exception : startface<0 || startface>=f_num || endface<0 || endface>=f_num || startface==endface");
					distMatrix[startface][geodesicNum[startface]].dist=f[startface].center.dist(middle)+f[endface].center.dist(middle);
					distMatrix[startface][geodesicNum[startface]].toFaceIndex=endface;
					distMatrix[startface][geodesicNum[startface]].times=1;
					geodesicNum[startface]++;
					distMatrix[endface][geodesicNum[endface]].dist=f[startface].center.dist(middle)+f[endface].center.dist(middle);
					distMatrix[endface][geodesicNum[endface]].toFaceIndex=startface;
					distMatrix[endface][geodesicNum[endface]].times=1;
					geodesicNum[endface]++;
					flag=true;
				}
			}
			if(!flag)
			{
				nearby[x[now]][nearbynum[x[now]]].f1Index=i;
				nearby[x[now]][nearbynum[x[now]]].toVertexIndex=x[endp];
				nearbynum[x[now]]++;
			}
		}

	}

	//这一部分程序用来通过dijkstra的方法扩展邻居面。基本就是扩展-比较的过程，没什么可说的。
	//其实结果也不是很本质，用测地距离有时候效果反而更好
	utilTool u;
	short int i0,j,k,jFace,kFace,findit;
	int* geodesicNum_backup=(int*)malloc(sizeof(int)*f_num);
	for(int sum=0;sum<5;sum++)
	{
		for(i=0;i<f_num;i++) geodesicNum_backup[i]=geodesicNum[i];
		for(i=0;i<f_num;i++)
		{
			for(j=0;j<geodesicNum_backup[i];j++)
			{
				jFace=distMatrix[i][j].toFaceIndex;
				if(jFace==i)
				{
					AfxMessageBox("Exception: jFace=i");
					continue;
				}
				for(k=0;k<geodesicNum_backup[jFace];k++) if(distMatrix[jFace][k].times==1)
				{
					kFace=distMatrix[jFace][k].toFaceIndex;
					if(kFace==i)continue;
					flag=false;
					for(i0=0;i0<geodesicNum[i];i0++) if(distMatrix[i][i0].toFaceIndex==kFace)
					{
						if (flag) AfxMessageBox("Exception : flag=true");
						flag=true;
						if(distMatrix[i][j].dist+distMatrix[jFace][k].dist<distMatrix[i][i0].dist)distMatrix[i][i0].times=distMatrix[i][j].times+1;
						distMatrix[i][i0].dist=u.getMin(distMatrix[i][i0].dist,distMatrix[i][j].dist+distMatrix[jFace][k].dist);
						for(findit=0;findit<geodesicNum[kFace];findit++) if(distMatrix[kFace][findit].toFaceIndex==i)
						{
							distMatrix[kFace][findit].dist=distMatrix[i][i0].dist;
							distMatrix[kFace][findit].times=distMatrix[i][i0].times;
							break;
						}
						if(flag)break;
					}
					if(!flag)
					{
						if(i==kFace) AfxMessageBox("Exception : i=kFace");
						distMatrix[i][geodesicNum[i]].dist=distMatrix[i][j].dist+distMatrix[jFace][k].dist;
						distMatrix[i][geodesicNum[i]].toFaceIndex=kFace;
						distMatrix[i][geodesicNum[i]].times=distMatrix[i][j].times+1;
						geodesicNum[i]++;
						distMatrix[kFace][geodesicNum[kFace]].dist=distMatrix[i][j].dist+distMatrix[jFace][k].dist;
						distMatrix[kFace][geodesicNum[kFace]].toFaceIndex=i;
						distMatrix[kFace][geodesicNum[kFace]].times=distMatrix[i][j].times+1;
						geodesicNum[kFace]++;
					}
				}
			}
		}
	}

	free(nearby);


	//tempF为邻域差值计算结果
	//tempF2为高斯积分计算结果

	double* tempF=(double*)malloc(sizeof(double)*f_num);
	double* tempF2=(double*)malloc(sizeof(double)*f_num);
	double avgdist=0,sumup=0;
	for(i=0;i<f_num;i++)
		for(j=0;j<geodesicNum[i];j++)
		{
			sumup+=1;
			avgdist+=distMatrix[i][j].dist;
		}
	avgdist/=sumup;

	double summ,alph;

	//计算邻域差值
	for(i=0;i<f_num;i++)
	{
		tempF[i]=0;
		summ=0;
		for(j=0;j<geodesicNum[i];j++)
		{
			int t=distMatrix[i][j].toFaceIndex;
			alph=1;
			summ+=alph;
			if(fColor[i].tag<fColor[t].tag) tempF[i]+=alph*0.00*(fColor[t].tag-fColor[i].tag);
			else	tempF[i]+=alph*(fColor[i].tag-fColor[t].tag);///fColor[i].tag*100;

		}
		tempF[i]/=summ;
	}

	//高斯积分
	for(i=0;i<f_num;i++)
	{
		tempF2[i]=0;
		summ=0;
		for(j=0;j<geodesicNum[i];j++)
		{
			int t=distMatrix[i][j].toFaceIndex;
			alph=exp(-u.double2(distMatrix[i][j].dist/(avgdist*bandwidth)));
			summ+=alph;
			if(tempF[t]>tempF[i])tempF2[i]+=alph*tempF[t];///fColor[i].tag*100;
		}

	}

	//根据程序需要USEINTEGRAL转化为最终结果
	maxf=0;minf=MAXdouble;
	for(i=0;i<f_num;i++) 
	{
		if(USEINTEGRAL==1)fColor[i].tag=tempF2[i];else fColor[i].tag=tempF[i];
		if(fColor[i].tag>maxf)maxf=fColor[i].tag;
		if(fColor[i].tag<minf)minf=fColor[i].tag;
	}
	for(i=0;i<f_num;i++)
	{
		fColor[i].tag-=minf;
		fColor[i].tag/=(maxf-minf);
		if(USEBOUNDARY==1)if(fColor[i].tag>f_color_boundary)fColor[i].tag=175;else fColor[i].tag=1;
	}
	if(USEBOUNDARY==1)
	{
		maxf=255;
		minf=0;
	}

	
	for(i=0;i<p_num;i++)pColor[i].tag=0;
	for(i=0;i<f_num;i++)
	{
		pColor[f[i].p1index].tag+=fColor[i].tag;
		pColor[f[i].p2index].tag+=fColor[i].tag;
		pColor[f[i].p3index].tag+=fColor[i].tag;
	}


	free(tempF);
	free(geodesicNum);
	free(geodesicNum_backup);

}

void myMain::loadMesh(CSceneGraph3d* pSceneGraph)
{
	pSceneGraph->RemoveAll();
	CTransform transform;
	transform.Clear();

	transform.SetTranslation(CVector3d(1,1,1));
	transform.SetRotation(CVector3d(0.5773,0.5773,0.5773));
	transform.SetScale(CVector3d(0.1,0.1,0.1));

	CMaterial material;
	CMaterial materialDebug;
	CMaterial materialOriginface;

	CMesh3d *pMesh = new CMesh3d;
	if(DEBUGMODE!=CHECKVISIBILITY)pMeshDebug=new CMesh3d;
	CMesh3d *pMeshOriginface= new CMesh3d;
	
	pSceneGraph->Add(pMesh);


	// Set Size (faster)
	pMesh->m_ArrayVertex.SetSize(p_num);
	pMeshDebug->m_ArrayVertex.SetSize(p_num);
	pMeshOriginface->m_ArrayVertex.SetSize(3);
	if(COLORFACE==1)pMesh->SetColorBinding(COLOR_PER_FACE);
	else {pMesh->SetColorBinding(COLOR_PER_VERTEX);}
	pMeshDebug->SetColorBinding(COLOR_PER_FACE);
	pMeshOriginface->SetColorBinding(COLOR_PER_FACE);

	// Store mesh (offset in file)
	CVertex3d* newp;
	for(int i=0;i<p_num;i++)
	{
		newp=new CVertex3d(p[i].x,p[i].y,p[i].z);
		newp->SetColor(pColor[i].r,pColor[i].g,pColor[i].b);
		pMesh->m_ArrayVertex.SetAt(i,newp);
		pMeshDebug->m_ArrayVertex.SetAt(i,new CVertex3d(p[i].x,p[i].y,p[i].z));
	}
	pMeshOriginface->m_ArrayVertex.SetAt(0,new CVertex3d(p[f[selectedShowFace].p1index].x,p[f[selectedShowFace].p1index].y,p[f[selectedShowFace].p1index].z));
	pMeshOriginface->m_ArrayVertex.SetAt(1,new CVertex3d(p[f[selectedShowFace].p2index].x,p[f[selectedShowFace].p2index].y,p[f[selectedShowFace].p2index].z));
	pMeshOriginface->m_ArrayVertex.SetAt(2,new CVertex3d(p[f[selectedShowFace].p3index].x,p[f[selectedShowFace].p3index].y,p[f[selectedShowFace].p3index].z));

	if(DEBUGMODE==CHECKVISIBILITY)
	{
		pSceneGraph->Add(pMeshDebug);
		pSceneGraph->Add(pMeshOriginface);
		int now=0, now1=0;
		for(int i=0;i<f_num;i++) if(i!=selectedShowFace && Visible[i]) now++;
		pMeshDebug->m_ArrayFace.SetSize(1);
		pMeshOriginface->m_ArrayFace.SetSize(1);
		pMesh->m_ArrayFace.SetSize(f_num);
		CFace3d *pFace = new CFace3d(pMesh->m_ArrayVertex.GetAt(f[selectedShowFace].p1index),
			pMesh->m_ArrayVertex.GetAt(f[selectedShowFace].p2index),
			pMesh->m_ArrayVertex.GetAt(f[selectedShowFace].p3index));
		CLine3d *pLine;


		pMeshOriginface->m_ArrayFace.SetAt(0,pFace);
		pMeshDebug->m_ArrayFace.SetAt(0,pFace);
		CVertex3d* center=new CVertex3d(f[selectedShowFace].center.x,f[selectedShowFace].center.y,f[selectedShowFace].center.z);
		now1=1;now=0;
		pFace->SetColor(255,0,0);
		pMesh->m_ArrayFace.SetAt(0,pFace);

		pMeshOriginface->m_ArrayLine.SetSize(4);
		myVector newPoint=f[selectedShowFace].getCenter().add(f[selectedShowFace].getNormal().Normalize().productReal(3.0f));
		pLine=new CLine3d(center ,new CVertex3d(newPoint.x,newPoint.y,newPoint.z));
		pMeshOriginface->m_ArrayLine.SetAt(0,pLine);

		newPoint=f[selectedShowFace].center.add(myVector(1,0,0).productReal(5.0f));
		pLine=new CLine3d(center ,new CVertex3d(newPoint.x,newPoint.y,newPoint.z));
		pMeshOriginface->m_ArrayLine.SetAt(1,pLine);

		newPoint=f[selectedShowFace].center.add(myVector(0,1,0).productReal(10.0f));
		pLine=new CLine3d(center ,new CVertex3d(newPoint.x,newPoint.y,newPoint.z));
		pMeshOriginface->m_ArrayLine.SetAt(2,pLine);

		newPoint=f[selectedShowFace].center.add(myVector(0,0,1).productReal(20.0f));
		pLine=new CLine3d(center ,new CVertex3d(newPoint.x,newPoint.y,newPoint.z));
		pMeshOriginface->m_ArrayLine.SetAt(3,pLine);

		for(int i=0;i<f_num;i++)if(i!=selectedShowFace)
		{
			pFace=new CFace3d(pMesh->m_ArrayVertex.GetAt(f[i].p1index), 
												   pMesh->m_ArrayVertex.GetAt(f[i].p2index),
												   pMesh->m_ArrayVertex.GetAt(f[i].p3index));
			pMesh->m_ArrayFace.SetAt(now1,pFace);now1++;
		}

	}
	else
	{
		pMesh->m_ArrayFace.SetSize(f_num);
		for(int i=0;i<f_num;i++)
		{
			CFace3d *pFace = new CFace3d(pMesh->m_ArrayVertex.GetAt(f[i].p1index),
				pMesh->m_ArrayVertex.GetAt(f[i].p2index),
				pMesh->m_ArrayVertex.GetAt(f[i].p3index));

			//pFace->SetColor(fColor[i].r,fColor[i].g,fColor[i].b);
			pMesh->m_ArrayFace.SetAt(i,pFace);
		}
	}

	// Transform & material
	pMesh->SetTransform(transform);
	pMeshDebug->SetTransform(transform);
	pMeshOriginface->SetTransform(transform);

	float alpha=1.0f;
	if(DEBUGMODE==CHECKVISIBILITY) 
	{
		alpha=0.85f;
	}
	material.SetAmbient(0.174500, 0.011750, 0.011750, alpha);
	material.SetDiffuse(0.614240, 0.041360, 0.041360, alpha);
	material.SetSpecular(0.727811, 0.626959, 0.626959, alpha);

	material.SetShininess(206.800003);
	material.SetEmission(0.6,0.0,0.0,alpha);
	pMesh->SetMaterial(&material);
	pMesh->SetName(CString("pMesh"));

	materialDebug.SetAmbient(0,0,1.0f,1.0f);
	materialDebug.SetDiffuse(0,0,1.0f,1.0f);
	materialDebug.SetSpecular(0,0,1.0f,1.0f);
	pMeshDebug->SetMaterial(&materialDebug);
	pMeshDebug->SetName(CString("pMeshDebug"));

	materialOriginface.SetAmbient(0,1.0f,0,1.0f);
	materialOriginface.SetDiffuse(0,1.0f,0,1.0f);
	materialOriginface.SetSpecular(0,1.0f,0,1.0f);
	pMeshOriginface->SetMaterial(&materialOriginface);
	pMeshOriginface->SetName(CString("pMeshOriginface"));
}

void myMain::main(char* fileName,CSceneGraph3d *pSceneGraph, int debugmode)
{


	myMain::DEBUGMODE=debugmode;

	filename=fileName;
	conv_filename();
	readfile();
			
	computeVIV(false);
	
	if(USEJUNCTIONDETECTION==1)junction_detection();
	if(DEBUGMODE!=CHECKVISIBILITY)computeResult();

	loadMesh(pSceneGraph);

	AfxMessageBox("Loading Finished");
	if(DEBUGMODE==NODEBUG)output_ply();
}

