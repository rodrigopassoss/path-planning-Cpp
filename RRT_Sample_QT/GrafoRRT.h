#ifndef GRAFORRT_H
#define GRAFORRT_H

//OpenCV
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/calib3d/calib3d.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

//OUTROS
#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <list>
#include <algorithm>

using namespace cv;
using namespace std;

class rrt_graph
{
private:
    vector<Point> nodes;
    vector<list<int>*> adj;   //Lista de ajacências
    Point goal;
    bool init_to_goal;
    int numNodes;

public:

    //Construtores
    rrt_graph()
    {
        this->numNodes = 0;
    }

    rrt_graph(Point init, Point _goal = Point(0,0), bool _init_to_goal = false)
    {
        nodes.push_back(init);
        this->goal = _goal;
        this->init_to_goal = _init_to_goal;
        adj.push_back(new list<int>);
        this->numNodes = 1;
    }

    //Destrutor
    ~rrt_graph()
    {
        for(int i = 0; i<numNodes;i++)
        {
            delete adj[i];
        }
    }

    //Funções Add
    void add_vertice(Point node)
    {
        nodes.push_back(node);
        adj.push_back(new list<int>);
        numNodes++;
    }
    void add_aresta(int index_source, int index_target)
    {
        adj[index_source]->push_back(index_target);
    }

    //Funções Get
    Point GetNodePoint(int index_node) const
    {
        return nodes[index_node];
    }
    list<int> GetNodeNeighbors(int index_node) const
    {
        return *adj[index_node];
    }
    int GetNumNodes() const
    {
        return numNodes;
    }


//Implementações Do movimento da RRT - São as implementações que variam com a técnica ultilizda

    //Método 'random_state' sorteia um ponto no mapa por distribuição uniforme
    Point random_state(Mat mapa)
    {
        return Point((rand() % mapa.cols),(rand() % mapa.rows));
    }

    //Sobrecarda de 'random_state'
    Point random_state(Mat mapa, Point goal, float epislon)
    {
        Point aux;
        if(((float)rand()/RAND_MAX) < epislon)
            aux = Point((rand() % mapa.cols),(rand() % mapa.rows));
        else
            aux = goal;

        return aux;
    }

    //Método 'nearest_neighbor' retorna o id do nó mas próximo do ponto 'x_rand'
    int nearest_neighbor(Point x_rand)
    {
        double min_dist = 99999.9;
        int nearest_node = 0;
        for (int node = 0; node < GetNumNodes() ; node++)
        {
            double sum = 0.0, dist;

            sum = pow(GetNodePoint(node).x - x_rand.x, 2.0)
                    + pow(GetNodePoint(node).y - x_rand.y, 2.0);

            dist = sqrt(sum);

            if (dist < min_dist)
            {
                min_dist = dist;
                nearest_node = node;
            }

        }

        return nearest_node;
    }

    //Método 'move' determina como a arvore vai expandir
    Point move(Point source, Point target)
    {
        //Encontrar orientação
        target.x = target.x - source.x;
        target.y = target.y - source.y;
        double angle = atan2(target.x,target.y);
        if(angle < 0)
            angle = angle + 2*M_PI;

       // cout << angle << endl;

        Point2f delta;

        delta.x = sin(angle);
        delta.y = cos(angle);

       // cout <<"delta antes: " << delta << endl;


        //Movimento na direção X
        if((0.0 < delta.x)&&(delta.x < 0.3535))
            delta.x = 0;
        else if((0.3535 < delta.x)&&(delta.x < 1))
            delta.x = 1;
        else if((-0.3535 < delta.x)&&(delta.x < 0))
            delta.x = 0;
        else if((-1 < delta.x)&&(delta.x < -0.3535))
            delta.x = -1;

        //Movimento na direção Y
        if((0.0 < delta.y)&&(delta.y < 0.3535))
            delta.y = 0;
        else if((0.3535 < delta.y)&&(delta.y < 1))
            delta.y = 1;
        else if((-0.3535 < delta.y)&&(delta.y < 0))
            delta.y = 0;
        else if((-1 < delta.y)&&(delta.y < -0.3535))
            delta.y = -1;

        //cout <<"delta depois: " << delta << endl;


        return delta;

    }

    //Método 'new_state' implementa o método 'move' para retornar o ponto x_new
    Point new_state(Point x_near, Point x_rand)
    {
        return Point(x_near.x + move(x_near, x_rand).x, x_near.y + move(x_near, x_rand).y);
    }

    //Método 'show' retorna uma matriz com o plot da árvore
    Mat show(Mat mapa)
    {
        for(int node = 0; node < numNodes; node++)
        {
            circle(mapa,GetNodePoint(node),2,Scalar(0,255,255),1);

            //list<int>::iterator it;

            /*for(it = GetNodeNeighbors(node).begin(); it != GetNodeNeighbors(node).end(); it++)
            {
                line(mapa,GetNodePoint(node),GetNodePoint(*it),Scalar(0,0,255),2);
            }*/
        }

        return mapa;
    }



};


#endif // GRAFORRT_H
