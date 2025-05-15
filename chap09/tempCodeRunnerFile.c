while (resource_count == 0)
        {
            pthread_cond_wait(&resource_cond, &resource_lock); 
        }