struct HEVert;
struct HEFace;
#include<vector>

struct v2
{
    float x;
    float y;

    v2(){x, y = 0; };

    v2(float p1, float p2)
    {
        x = p1;
        y = p2;
    }

    v2(const v2& a)
    {
        x = a.x;
        y = a.y;
    }
    
    bool operator==(const v2& one)
    {
        if(x == one.x && y == one.y)
            return true;
        return false;
    }

    v2 operator+(v2 one) const
    {
        return v2(this->x + one.x, this->y + one.y);
    }
    v2 operator*(float mult) const
    {
        return v2(this->x * mult, this->y * mult);
    }
};

struct v3
{
    float x;
    float y;
    float z;

    v3(){x = 0; y = 0; z = 0;};
  
    v3(float p1, float p2, float p3)
    {
        x = p1;
        y = p2;
        z = p3;
    }

    v3(const v3& a)
    {
        x = a.x;
        y = a.y;
        z = a.z;
    }

    bool operator==(const v3& one)
    {
        if(one.x == x && one.y == y && one.z == z)
            return true;
        return false;
    }

    v3 operator+(v3 one) const
    {
        return v3(this->x + one.x, this->y + one.y, this->z + one.z);
    }
    v3 operator-(v3 one) const
    {
        return v3(this->x - one.x, this->y - one.y, this->z - one.z);
    }
    v3 operator*(float mult) const
    {
        return v3(this->x * mult, this->y * mult, this->z * mult);
    }
    v3 operator*(v3 cross) const
    {
        v3 r;

        r.x = this->y*cross.z - cross.y*this->z;
        r.y = this->z*cross.x - cross.z*this->x;
        r.z = this->x*cross.y - cross.x*this->y;
        
        return r;
    }

    v3 operator/(float div) const
    {
        return v3(this->x / div, this->y / div, this->z / div);
    }
  
};

struct HEEdge
{
    HEVert* vert; //Vertex at the end of the half-edge
    HEEdge* pair; //Oppositely oriented adjacent half-edge
    HEFace* face; //Face the half-edge borders
    HEEdge* next; //next half-edge around the face
    HEVert *ePt;
    v2 uv;
    v3 norm;

    bool split;
    
    HEEdge()
    {
        split = false;
        vert = nullptr;
        pair = nullptr;
        face = nullptr;
        next = nullptr;
        ePt = nullptr;
    };
};

struct HEVert
{
    v3 pos;

    HEVert()
    {
        edge = nullptr;
        vNew = nullptr;
    };
    HEVert(v3 v)
    {
        pos = v;
        edge = nullptr;
    };

    HEEdge* edge; //One of the half-edges emantating from the vertex
    HEVert *vNew;
    
    bool operator==(HEVert& one)
    {
        if(one.pos == pos)
            return true;
        return false;
    };

};

struct HEFace
{
    HEEdge* edge; //One of the half-edges bordering the face

    HEVert *avg;
    
    HEFace()
    {
        edge = nullptr;
        avg = nullptr;
    };
};

class Mesh
{
private:
    HEEdge* entry;
    HEVert* vList;
    HEFace* fList;
    int fCount;
    int eStep;
    int vCount;
    std::vector<v2> debugPoints;
public:
    Mesh();
    ~Mesh();
    void generateHalfEdge(char*);
    v3* getVB();
    v2* getUV();
    v3* getNormals();
    int VBLen(){ return fCount*eStep; };
    int ePerF(){ return eStep; };
    void subdivide();
    void fuckmeup();
    void deleteHalfEdge();
    void drawWire();
    void debugDraw();
};
