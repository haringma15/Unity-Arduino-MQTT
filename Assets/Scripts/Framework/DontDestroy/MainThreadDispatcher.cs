using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainThreadDispatcher : MonoBehaviour
{
    private static MainThreadDispatcher instance;
    private Queue<Action> actionQueue = new();

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }

        StartCoroutine(ActOnMainThread());
    }


    public static void RunOnMainThread(Action action)
    {
        if (instance == null) return;
        lock (instance.actionQueue) instance.actionQueue.Enqueue(action);
    }

    IEnumerator ActOnMainThread()
    {
        for (;;)
        {
            lock (actionQueue)
            {
                while (actionQueue.Count > 0)
                {
                    Action action = actionQueue.Dequeue();
                    action.Invoke();
                }
            }

            yield return null;
        }
    }
}
