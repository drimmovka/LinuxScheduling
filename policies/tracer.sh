#!/bin/bash

echo function > nodev/current_tracer
echo 1 > nodev/tracing_on

sleep 1

less nodev/trace

echo nop > nodev/current_tracer
echo 0 > nodev/tracing_on