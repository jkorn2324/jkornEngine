#pragma once

#include "SystemManager.h"
#include "IEventSystem.h"

namespace Engine
{
    class Timestep;
    struct UpdateSystemContext;

    namespace SystemUtility
    {
        /**
         * Invokes the OnUpdate function for the system manager. 
         */
        void InvokeOnUpdate(const Engine::Timestep& timestep, const bool isPlaying);
        /**
         * Invokes the OnUpdate function for the system.
         */
        void InvokeOnUpdate(const Engine::UpdateSystemContext& context);
        
        /**
         * Invokes the event using the context.
        */
        template<typename TContext>
        void InvokeEvent(const TContext& context)
        {
            using EventSystemType = typename IEventSystem<TContext>;
            SystemManager::Invoke<EventSystemType, TContext>(
                (EventSystemType& eventSystem, const TContext& ctx) -> void {
                    eventSystem.InvokeEventTriggered(ctx);
                });
        }
    }
}