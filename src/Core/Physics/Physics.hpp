#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <vector>
#include "../../Game/Game.h"

using namespace reactphysics3d;

class PhysicsEngine
{
    PhysicsCommon physicsCommon;
    PhysicsWorld* world = physicsCommon.createPhysicsWorld();
    RigidBody* groundBody; // Added ground body
    std::vector<RigidBody*> bodies;

public:
    void Init()
    {
        // Set the gravity vector
        Vector3 gravity(0, -9.81, 0);
        world->setGravity(gravity);

        world->setIsDebugRenderingEnabled(true);
        DebugRenderer& debugRenderer = world->getDebugRenderer();

        // Select the contact points and contact normals to be displayed
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_NORMAL, true);

        // Create a ground rigid body
        Vector3 groundPosition(0, -1, 0); // Adjust the ground position
        Transform groundTransform(groundPosition, Quaternion::identity());
        groundBody = world->createRigidBody(groundTransform);
        groundBody->setType(BodyType::STATIC);
        groundBody->setIsDebugEnabled(true);


        // Create a material using the properties

        // Create a box collision shape for the ground
        Vector3 groundHalfExtents(50.0, 0.5, 50.0); // Adjust the size of the box
        BoxShape* groundShape = physicsCommon.createBoxShape(groundHalfExtents);
        groundBody->addCollider(groundShape, Transform::identity());
    }

    void Update(float deltaTime)
    {
        world->update(deltaTime);

        for (auto& body : bodies)
        {
            if (body != nullptr)
            {
                const Transform& transform = body->getTransform();
                const Vector3& position = transform.getPosition();
            }
        }
    }

    RigidBody* CreateBody(Vector3 position, Quaternion orientation, Vector3 halfExtents)
    {
        RigidBody* body;
        BoxShape* actorShape = physicsCommon.createBoxShape(halfExtents);
        body = world->createRigidBody({ position, orientation });

        body->setType(BodyType::DYNAMIC);
        body->setMass(5);
        body->addCollider(actorShape, Transform::identity());
        bodies.push_back(body);

        return body;
    }


    RigidBody* CreateBodyFromConvexMesh(float* vertices, int Numvertices, Vector3 position, Quaternion orientation)
    {
        std::cout << "1" << std::endl;
        RigidBody* body;
        // Vertex array with all vertices of the mesh
        rp3d::VertexArray vertexArray(vertices, 3 * sizeof(float), Numvertices, rp3d::VertexArray::DataType::VERTEX_FLOAT_TYPE);

        std::cout << "2" << std::endl;
        std::vector < rp3d::Message > messages;
        ConvexMesh* convexMesh = physicsCommon.createConvexMesh(vertexArray, messages);
        std::cout << "3" << std::endl;

        //if (messages.size() > 0) {
        //    for (const rp3d::Message& message : messages) {
        //        std::string messageType;
        //        switch (message.type) {
        //        case rp3d::Message::Type::Information:
        //            messageType = " info ";
        //            break;
        //        case rp3d::Message::Type::Warning:
        //            messageType = " warning ";
        //            break;
        //        case rp3d::Message::Type::Error:
        //            messageType = " error ";
        //            break;
        //        }
        //        std::cout << " Message (" << messageType << "): " <<
        //            message.text << std::endl;
        //    }
        //}
        // Make sure there was no errors during mesh creation
        assert(convexMesh != nullptr);
        std::cout << "4" << std::endl;
        // Create a convex mesh shape from the polygon vertex array
        ConvexMeshShape* actorShape = physicsCommon.createConvexMeshShape(convexMesh);
        std::cout << "5" << std::endl;

        // Create a rigid body with the specified position and orientation
        body = world->createRigidBody({ position, orientation });

        // Set the body type to dynamic
        body->setType(BodyType::DYNAMIC);

        // Set the mass of the body
        body->setMass(5);

        // Add the convex mesh shape as a collider to the body
        body->addCollider(actorShape, Transform::identity());

        // Add the body to the list of bodies
        bodies.push_back(body);

        return body;
    }

private:
    // ...
};