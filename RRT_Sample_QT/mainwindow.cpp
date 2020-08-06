#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mapa->setScaledContents(true);
    srand (time(NULL));

    _mapa = "rect10.png";
    mapa_show = imread(_mapa);
    mapa_show.copyTo(mapa_operate);

    //mapa_operate = Mat(mapa_show.rows,mapa_show.cols, CV_8UC1);
    //inRange(mapa_show,Scalar(200,200,200),Scalar(255,255,255),mapa_operate);
    update_map(mapa_show);

    cout << "Colunas: " << mapa_show.cols <<  endl;
    cout << "Linhas: " << mapa_show.rows <<  endl;


    connect(ui->mapa, SIGNAL(Mouse_pos()), this, SLOT(Mouse_current_pos()));
    connect(ui->mapa, SIGNAL(Mouse_pressed()), this, SLOT(Mouse_pressed()));



}

MainWindow::~MainWindow()
{
    delete rrt;
    delete ui;
}

void MainWindow::update_map(Mat img)
{
    QImage image = QImage((uchar*) img.data, img.cols, img.rows,img.step, QImage::Format_BGR888);
    ui->mapa->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::Mouse_current_pos()
{
    ui->XY_pos->setText(QString("X = %1, Y = %2").arg(ui->mapa->x).arg(ui->mapa->y));

}

void MainWindow::Mouse_pressed()
{
    if((chv1 == true && chv2 == true))
    {
        mapa_show = imread(_mapa);
        mapa_show.copyTo(mapa_operate);
    }

    if(ui->init_goal->currentIndex() == 0)
    {
        chv1 = !chv1;
        //Inicializa o init
        init = Point(ui->mapa->x_press,ui->mapa->y_press);
        circle(mapa_show,init,4,Scalar(255,0,0),2);
        circle(mapa_show,init,2,Scalar(255,255,255),1);
        putText(mapa_show,"INIT", Point(init.x + 7, init.y + 7),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,0,255),1,LINE_AA);

    }
    else if(ui->init_goal->currentIndex() == 1)
    {
        chv2 = !chv2;
        //inicializa o goal
        goal = Point(ui->mapa->x_press,ui->mapa->y_press);
        circle(mapa_show,goal,4,Scalar(0,0,255),2);
        circle(mapa_show,goal,2,Scalar(255,255,255),1);
        putText(mapa_show,"GOAL", Point(goal.x + 7, goal.y + 7),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,0,255),1,LINE_AA);

    }

    update_map(mapa_show);
}

void MainWindow::RRT()
{

    //Raiz da RRT
    //init = Point(27,17);
    rrt = new rrt_graph(init);
    update_map(mapa_show);

        for (int k = 1;k<1000;k++)
        {
            //cout << k << endl;
            cout << rrt->GetNumNodes() << endl;

            //Sorteia um Ponto aleatório
            Point x_rand = rrt->random_state(mapa_show,goal,0.6);
            //cout << " x_rand: " << x_rand << endl;

            //Calcula o visinho(no grafo) mais próximo do ponto sorteado
            //Define x_near como o nó mais próximo de x_rand;
            int nearest_node = rrt->nearest_neighbor(x_rand);
            Point x_near = rrt->GetNodePoint(nearest_node);
            //cout<< "x_near: " << x_near << endl;


            //Movimento na direção de x_rand
            Point x_new = rrt->new_state(x_near,x_rand);

            //cout<< "x_new: " << x_new << endl;


            //Verifica se o ponto escolhido pertence ao X_free
            if((int)mapa_operate.at<Vec3b>(x_new.x,x_new.y)[2]==0)
            {
                //Verifica se x_new já é um nó do grafo
                bool existis_node = false;
                for (int i = 0;i < rrt->GetNumNodes();i++)
                {
                    if(x_new == rrt->GetNodePoint(i))
                    {
                        existis_node = true;
                        break;
                    }
                }
                if(existis_node == false)
                {
                    //Adiciona vértice à arvore
                    rrt->add_vertice(x_new);
                    //Adiciona Aresta
                    rrt->add_aresta(nearest_node,(rrt->GetNumNodes()-1));

                    //if(x_new == goal)
                       // break;

                }

            }

        }

        update_map(rrt->show(mapa_show));


        cout << "Fim RRT" << endl;

}


void MainWindow::on_GoRRT_clicked()
{
    /*for(int i = 0; i < mapa_show.cols; i++)
    {
        for(int j = 0; j < mapa_show.rows; j++)
        {
            if((int)mapa_show.at<Vec3b>(j,i)[2])
                cout << (int)mapa_show.at<Vec3b>(j,i)[2] << endl;
        }
    }*/
    RRT();
}

void MainWindow::on_pushButton_clicked()
{
    delete rrt;
    mapa_show = imread(_mapa);
    mapa_show.copyTo(mapa_operate);
    update_map(mapa_show);
}
