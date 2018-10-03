#pragma once
#include "Surface.h"
#include "ColorTemplate.h"
#include <vector>
#include <memory>

namespace CommonClass
{

/*!
    \brief Polygon represent a shape whose vetices lying on the same plane defined by ( * x + d = 0)
    You must keep the vertices in the same plane
*/
class Polygon
    :public Surface
{
public:
    /*!
        \brief all my vertices of the Polygon, at least three points.
    */
    std::vector<vector3> m_points;

    /*!
        \brief the normal of the plane, define by the first three vertices in the m_points.
        p0, p1, p2,
        e1 = p1 - p0
        e2 = p2 - p0
        m_normal = Normalize(e1 X e2);
    */
    vector3 m_normal;

    /*!
        \brief the distanct from plane to origin
        this is a key element to define a plane.
    */
    Types::F32 m_d;

    /*!
        \brief the precomputed bounding box.
    */
    AABB m_boundingBox;


protected:
    /*!
        \brief precompute which plane to project in the hit function
        for example, if the maxmun component of m_normal is y axis,
        then the plane should be project into XZ plane:
        and the m_index0 = 0, m_index1 = 2;
        if project into XY plane:
        m_index0 = 0, m_index1 = 1;
    */
    unsigned int m_index0, m_index1;

public:
    /*!
        \brief build a polygon plane throught three points.
        \param p0 - 2 three point position
        clockwise is front face.
    */
    Polygon(const vector3& p0, const vector3& p1, const vector3& p2);
    ~Polygon();

    /*!
        \brief append a point into the polygon.
        \param p appended point position
        the class will project the new point into the plane.
    */
    void AddPoint(const vector3& p);
    
    /*!
        \brief try ray hit on this sphere
        \param ray hit ray
        \param t0 min T value
        \param t1 max T value
        \param pHitRec return the hit detail
    */
    virtual bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) const override;

    /*!
        \brief get bounding box of the sphere.
    */
    virtual AABB BoundingBox() const override;

protected:
    /*!
        \brief update the bounding box through all the points position.
        This function will be called at the constructor or after a new point added.
    */
    void BuildBoundingBox();
};

} // namespace CommonClass

