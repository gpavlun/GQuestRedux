#! /bin/bash
echo "## Project layout:" > architecture.md
# get structure of project with tree and use sed to get valid newlines for Markdown
tree ../. -a -I '.cache|.git|target|build/|bin/|.gitignore|*.o|*.d|*.a|world_data/*' | sed 's/$/  /' >> architecture.md

echo "  " >> architecture.md
echo "## Lines of code by file:  " >> architecture.md

find ../ \
  \( -path "../target" -o -path "../.git" -o -path "../build" \) -prune -o \
  \( -name "*.c" -o -name "*.h" -o -name "*.rs" \) \
  -exec wc -l {} + >> architecture.md