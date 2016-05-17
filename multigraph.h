#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include <QQueue>
#include <QDebug>
#include <QVector>
#include <QDataStream>
#include <QLinkedList>


template<typename NodeType, typename ArcType>
class MultiGraphIterator;

template<typename NodeType, typename ArcType>
class HGraphNode;

template<typename NodeType, typename ArcType>
class HGraphArc
{
public:
    HGraphArc(){
        m_node = NULL;
    }
    HGraphArc(ArcType p_weight) : m_weight(p_weight){m_node = NULL;}
    HGraphNode<NodeType, ArcType> *m_node;
    ArcType m_weight;
//    int m_nodeIndex;


    friend QDataStream &operator<<(QDataStream &ds, const HGraphArc<NodeType,ArcType> & rhs)
    {
        ds << rhs.m_weight;
        return ds;
    }

    friend QDataStream &operator>>(QDataStream &ds, HGraphArc<NodeType,ArcType> & rhs)
    {
        ds >> rhs.m_weight;
        return ds;
    }
};

template<typename NodeType, typename ArcType>
class HGraphNode
{
public:
    typedef HGraphArc<NodeType, ArcType> Arc;
    typedef HGraphNode<NodeType, ArcType> Node;

    NodeType m_data;
    QVector<Arc> m_arcList;
    Node* prev;
    bool m_marked;

    HGraphNode ():m_marked(false), prev(NULL){}

    HGraphNode (NodeType p_data) : m_data(p_data), m_marked(false), prev(NULL) {}

    HGraphNode (const HGraphNode & rhs)
    {
        //shallow copy
        m_data = rhs.m_data;
        m_marked = rhs.m_marked;
        //deep copy
        for(int i=0; i<rhs.m_arcList.size (); i++)
        {
            m_arcList << rhs.m_arcList[i];
        }
    }

    friend QDataStream &operator<<(QDataStream &ds, const Node *rhs)
    {
        ds << rhs->m_data;
        ds << rhs->m_arcList.size ();
        return ds;
    }

    friend QDataStream& operator>>(QDataStream &ds, Node *rhs)
    {
        ds >> rhs->m_data;

        int numberOfArcs = 0;
        ds >> numberOfArcs;
        return ds;
    }

    void AddArc(Arc p_arc)
    {
        m_arcList << p_arc;
        m_arcList.begin ();
    }

    void AddArc(Node* p_node, ArcType p_weight)
    {
        Arc a;
        a.m_node = p_node;
        a.m_node->m_marked = false;
        a.m_weight = p_weight;
        m_arcList << a;
    }

    bool operator== (const Node& p_rhs) const {
        return m_data == p_rhs.m_data;
    }

    bool operator!= (const Node& p_rhs) const{
        return m_data != p_rhs.m_data;
    }

    class xNoArcFound{};
    Arc* GetArc(Node* p_node)
    {
        for(int i=0; i<m_arcList.size (); i++)
        {
            if(m_arcList[i].m_node == p_node)
                return &m_arcList[i];
        }
        throw xNoArcFound();
        return NULL;
    }

    void RemoveArc(Node* p_node)
    {
        for(int i=0; i<m_arcList.size (); i++)
        {
            if(*m_arcList[i].m_node == *p_node)
                m_arcList.remove (i);
        }
    }

    void RemoveArc(Node* p_node, ArcType p_weight)
    {
        for(int i=0; i<m_arcList.size (); i++)
        {
            if(m_arcList[i].m_weight== p_weight)
                if(*m_arcList[i].m_node == *p_node)
                    m_arcList.remove (i);
        }
    }
};

template<typename NodeType, typename ArcType>
class MultiGraph
{
public:
    typedef HGraphArc<NodeType, ArcType> Arc;
    typedef HGraphNode<NodeType, ArcType> Node;

    QVector<Node*> m_nodes;
    int m_count;
    MultiGraph() : m_count(0){}
    MultiGraph(const MultiGraph &mul) // copy constructor for graph
    {
        m_count = mul.m_count;

        //deep copy
        for(int i=0; i<mul.m_nodes.size (); i++)
        {
            m_nodes << new Node(*mul.m_nodes.at (i));
        }
    }


    // Serialization begins here
    friend QDataStream& operator<<(QDataStream &ds, const MultiGraph& rhs)
    {
        //save the size of the graph first to use later for loading
        ds << rhs.m_count;

        //save all nodes first to memory
        for(int i=0; i<rhs.m_count; i++){
            ds << rhs.m_nodes.at (i);
        }

        //foreach node in the graph, save all arcs
        foreach(Node* n, rhs.m_nodes){
            //save node's arclist size to use later for loading
            ds << n->m_arcList.size ();
            //save all arcs here
            foreach(Arc arc, n->m_arcList){
                // save only the arc's weight
                ds << arc.m_weight;
                //instead of saving the arc's node data, save only it's index to prevent duplication
                ds << rhs.m_nodes.indexOf (arc.m_node);
            }
        }

        return ds;
    }

    // Deserilization begins here
    friend QDataStream& operator>>(QDataStream &ds, MultiGraph& rhs)
    {
        // clear graph data
        rhs.Clear ();

        int count = 0;
        ds >> count;
        // load all node data here
        for(int i=0; i<count; i++){
            Node* aNode = new Node();
            ds >> aNode;
            rhs.AddNode (aNode);
        }

        foreach (Node *n, rhs.m_nodes) {
            int arcListSize;
            ds >> arcListSize;
            for(int i=0; i<arcListSize; i++){
                Arc arc;
                ds >> arc.m_weight;
                int destNodeIdx;
                ds >> destNodeIdx;
                arc.m_node = rhs.m_nodes.at (destNodeIdx);
                n->AddArc (arc);
            }
        }

        return ds;
    }

    ~MultiGraph()
    {
        for(int i=0; i<m_nodes.size (); i++)
        {
            delete m_nodes[i];
        }
    }

    MultiGraphIterator<NodeType, ArcType> Iterator()
    {
        return MultiGraphIterator<NodeType, ArcType>(*this);
    }

    bool isEmpty(){
        return m_nodes.empty();
    }

    bool AddNode(NodeType p_data)
    {
        Node tempNode(p_data);
        for(int i=0; i<m_nodes.size (); i++){
            if(*m_nodes[i] == tempNode)
                return false;
        }

        Node *aNode = new Node(p_data);
        aNode->m_marked = false;
        m_nodes << aNode;
        m_count++;
        return true;
    }

    bool AddNode(Node *p_node){
        m_nodes << p_node;
        m_count++;
        return true;
    }

    void Clear()
    {
//        for(int i=0; i<m_nodes.size (); i++)
//        {
//            RemoveNode (i);
//        }
        m_nodes.clear ();
        m_count = 0;
    }

    void RemoveArc(NodeType p_from, NodeType p_to)
    {
        int idxFrom = m_count + 1;
        int idxTo = m_count + 1;

        try{
            GetFromToIndices (p_from, &idxFrom, p_to, &idxTo);
        } catch(xIndexOutOfRange){
            qDebug() << p_from << "not found";
            return ;
        } catch(xIndicesIdentical){
            qDebug() << "Error! cannot create arc on single node";
            return ;
        }

        RemoveArc (idxFrom, idxTo);
    }

    void RemoveArc(NodeType p_from, NodeType p_to, ArcType p_weight)
    {
        int idxFrom = m_count + 1;
        int idxTo = m_count + 1;

        try{
            GetFromToIndices (p_from, &idxFrom, p_to, &idxTo);
        } catch(xIndexOutOfRange){
            qDebug() << p_from << "not found";
            return ;
        } catch(xIndicesIdentical){
            qDebug() << "Error! cannot create arc on single node";
            return ;
        }

        if(m_nodes[idxFrom] == NULL || m_nodes[idxTo] == NULL)
            return;
        m_nodes[idxFrom]->RemoveArc (m_nodes[idxTo], p_weight);
    }

    void RemoveArc(int p_from, int p_to) const
    {
        if(m_nodes[p_from] == NULL || m_nodes[p_to] == NULL)
            return;
        m_nodes[p_from]->RemoveArc (m_nodes[p_to]);
    }

    void RemoveNode(NodeType p_data)
    {
        RemoveNode(GetIndex (p_data));
    }

    void RemoveNode(int p_index)
    {
        if(p_index > m_nodes.size ())
            return;
        if(m_nodes[p_index] == NULL)
            return;

        Arc* arc;

        for(int i=0; i<m_nodes.size (); i++){
            if(m_nodes[i] != NULL){
                try{
                    arc = m_nodes[i]->GetArc (m_nodes[p_index]);
                } catch (typename Node::xNoArcFound){
                    arc = NULL;
                }

                if(arc!=NULL){
                    RemoveArc (i, p_index);
                }
            }
        }
        delete m_nodes[p_index];
        m_nodes[p_index] = NULL;
        m_nodes.remove (p_index);
        m_count--;
    }

    // Exceptions
    class xIndexOutOfRange {};
    class xIndicesIdentical {};
    bool AddArc(NodeType p_from, NodeType p_to, ArcType p_weight)
    {
        int idxFrom = m_count + 1;
        int idxTo = m_count + 1;

        try{
            GetFromToIndices (p_from, &idxFrom, p_to, &idxTo);
        } catch(xIndexOutOfRange){
            qDebug() << p_from << "not found";
            return false;
        } catch(xIndicesIdentical){
            qDebug() << "Error! cannot create arc on single node";
            return false;
        }

        if (m_nodes[idxFrom] == 0 || m_nodes[idxTo] == 0)
            return false;
        //commenting this out ensures that it's possible for two nodes to have multiple arcs between them
//        try{
//            m_nodes[idxFrom]->GetArc(m_nodes[idxTo]);
//        } catch (...){
//            return false;
//        }
        m_nodes[idxFrom]->AddArc(m_nodes[idxTo], p_weight);
        return true;
    }

    Arc* GetArc(NodeType p_from, NodeType p_to)
    {
        int idxFrom;
        int idxTo;
        GetFromToIndices (p_from, &idxFrom, p_to, &idxTo);

        if (m_nodes[idxFrom] == 0 || m_nodes[idxTo] == 0)
            return NULL;

        Arc* arc = NULL;
        try{
            arc = m_nodes[idxFrom]->GetArc(m_nodes[idxTo]);
        } catch (typename Node::xNoArcFound){
            qDebug() << "no arc between"<< p_from << "and" << p_to;
        }

        return arc;
    }

    Node* GetNode(int p_index)
    {
        return m_nodes.at (p_index);
    }

    Node* GetNode(NodeType p_data)
    {
        Node aNode(p_data);
        for(int i=0; i<m_nodes.size (); i++){
            if(*m_nodes[i] == aNode)
                return m_nodes.at (i);
        }

        return NULL;
    }

    int GetIndex(NodeType p_data)
    {
        Node aNode(p_data);
        for(int i=0; i<m_nodes.size (); i++){
            if(*m_nodes[i] == aNode) return m_nodes.indexOf (m_nodes[i]);
        }
    }

    void GetFromToIndices(NodeType p_from, int *idx_from, NodeType p_to, int *idx_to)
    {
        Node nodeFrom(p_from);
        Node nodeTo(p_to);
        for(int i=0; i<m_nodes.size (); i++){
            if(*m_nodes[i] == nodeFrom) *idx_from = m_nodes.indexOf (m_nodes[i]);
            if(*m_nodes[i] == nodeTo) *idx_to = m_nodes.indexOf (m_nodes[i]);
        }
        if(*idx_from > m_count || *idx_to > m_count)
            throw xIndexOutOfRange();
        if(*idx_from == *idx_to)
            throw xIndicesIdentical();
    }

    void ClearMarks()
    {
        for (int index = 0; index < m_nodes.m_size; index++) {
            if (m_nodes[index] != 0) {
                m_nodes[index]->m_marked = false;
            }
        }
    }

    void DepthFirst(Node *p_node, void (*p_process)(Node*))
    {
        if(p_node == NULL)
            return;

        p_process(p_node);
        p_node->m_marked = true;

        for(typename QVector<Arc>::Iterator itr = p_node->m_arcList.begin ();
            itr != p_node->m_arcList.end (); itr++)
        {
            if(itr->m_node->m_marked == false){
                DepthFirst (itr->m_node, p_process);
            }
        }
    }

    void DepthFirst(Node *p_node, ArcType p_weight, void (*p_process)(Node*))
    {
        if(p_node == NULL)
            return;

        p_process(p_node);
        p_node->m_marked = true;

        for(typename QVector<Arc>::Iterator itr = p_node->m_arcList.begin ();
            itr != p_node->m_arcList.end (); itr++)
        {
            if(itr->m_weight == p_weight)
                if(itr->m_node->m_marked == false){
                    DepthFirst (itr->m_node, p_process);
                }
        }
    }

    void clearMarks()
    {
        for(int i=0; i<m_nodes.size(); i++)
        {
            m_nodes[i]->m_marked = false;
        }
    }

    void BreadthFirst(Node* p_node, void (*p_process)(Node*))
    {
        if(p_node == NULL)
            return;

        QQueue<Node*> queue;

        queue << p_node;
        p_node->m_marked = true;

        while(queue.size () != 0){
            p_process(queue.front ());
            for(typename QVector<Arc>::Iterator itr = queue.front ()->m_arcList.begin ();
                itr != queue.front ()->m_arcList.end (); itr++)
            {
                if(itr->m_node->m_marked == false){
                    itr->m_node->m_marked = true;
                    queue << itr->m_node;
                }
            }
            queue.dequeue ();
        }
    }

    QVector<Node*> FindPath(Node* p_node, bool(*p_process)(Node*))
    {
        QVector<Node*> path;
        if(p_node == NULL)
            return path;

        QQueue<Node*> queue;
        queue << p_node;
        p_node->m_marked = true;

        while(queue.size () != 0){
            if(p_process(queue.front ()))
            {
                QDebug d = qDebug();
                d << "path:";
                for(Node* itr = queue.front (); itr!=p_node->prev; itr=itr->prev){
                    d << itr->m_data.name();
                    path << itr;
                }

                return path;
            }
            for(typename QVector<Arc>::Iterator itr = queue.front ()->m_arcList.begin ();
                itr != queue.front ()->m_arcList.end (); itr++)
            {
                if(itr->m_node->m_marked == false){
                    itr->m_node->m_marked = true;
                    itr->m_node->prev = queue.front ();
                    queue << itr->m_node;
                }
            }
            queue.dequeue ();
        }
    }
};

template<typename NodeType, typename ArcType>
class MultiGraphIterator
{
public:
    typedef HGraphArc<NodeType, ArcType> Arc;
    typedef HGraphNode<NodeType, ArcType> Node;
    MultiGraphIterator(MultiGraph<NodeType,ArcType> &gr) : m_graph(gr){index = 0;}

    class xEmpty{};
    void begin()
    {
        m_node = m_graph.m_nodes.first ();
        index = 0;
    }

    void operator++()
    {
        index++;
    }

    void operator--()
    {
        index--;
    }

    bool isValid()
    {
        return index != m_graph.m_nodes.size ();
    }

    void end()
    {
        if(m_graph != NULL){
            m_node = m_graph->m_nodes.last ();
        }
    }

    Node* operator *()
    {
        m_node = m_graph.m_nodes[index];
        return m_node;
    }

private:
    const MultiGraph<NodeType, ArcType> &m_graph;
    Node* m_node;
    int index;
};

#endif // MULTIGRAPH_H
