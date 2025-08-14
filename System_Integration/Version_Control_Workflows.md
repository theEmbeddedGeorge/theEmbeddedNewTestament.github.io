# Version Control Workflows

## Overview
Version control workflows are essential for managing embedded software development in team environments. This guide covers Git-based version control strategies, branching models, release management, and best practices that enable collaborative development while maintaining code quality and project stability.

## Table of Contents
1. [Core Concepts](#core-concepts)
2. [Git Fundamentals](#git-fundamentals)
3. [Branching Strategies](#branching-strategies)
4. [Collaborative Workflows](#collaborative-workflows)
5. [Release Management](#release-management)
6. [Code Review Process](#code-review-process)
7. [Continuous Integration](#continuous-integration)
8. [Common Issues and Solutions](#common-issues-and-solutions)
9. [Best Practices](#best-practices)
10. [Interview Questions](#interview-questions)

---

## Core Concepts

### What is Version Control?
Version control systems enable developers to:
- **Track Changes**: Maintain history of all code modifications
- **Collaborate**: Work simultaneously on shared codebases
- **Manage Versions**: Organize releases and feature development
- **Rollback Changes**: Revert to previous working states
- **Branch Development**: Work on features without affecting main code

### Version Control Workflow Benefits
```
Workflow Benefits:
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Code      │───▶│   Team      │───▶│   Quality   │───▶│   Release   │
│  History    │    │  Collaboration│   │  Assurance  │    │  Management │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
        │                   │                   │                   │
        ▼                   ▼                   ▼                   ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Audit     │    │   Parallel  │    │   Automated │    │   Stable    │
│   Trail     │    │  Development│    │   Testing   │    │   Releases  │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
```

### Workflow Types
- **Centralized**: Single repository with linear development
- **Distributed**: Multiple repositories with flexible collaboration
- **Feature-based**: Development organized around features
- **Release-based**: Development organized around releases

---

## Git Fundamentals

### Basic Git Commands
```bash
# Repository initialization and setup
git init                    # Initialize new repository
git clone <url>            # Clone existing repository
git remote add origin <url> # Add remote origin

# Basic workflow commands
git add <file>             # Stage files for commit
git commit -m "message"    # Commit staged changes
git push origin <branch>   # Push commits to remote
git pull origin <branch>   # Pull latest changes

# Status and information
git status                 # Show working directory status
git log                    # Show commit history
git diff                   # Show unstaged changes
git branch                 # List local branches
git checkout <branch>      # Switch to branch
```

### Git Configuration
```bash
# Global configuration
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
git config --global core.editor "vim"
git config --global init.defaultBranch main

# Repository-specific configuration
git config user.name "Project Specific Name"
git config user.email "project@example.com"

# Useful aliases
git config --global alias.st status
git config --global alias.co checkout
git config --global alias.br branch
git config --global alias.ci commit
git config --global alias.unstage 'reset HEAD --'
git config --global alias.last 'log -1 HEAD'

# Credential management
git config --global credential.helper store
git config --global credential.helper cache --timeout=3600
```

### Git Ignore Configuration
```gitignore
# .gitignore for embedded projects
# Build artifacts
build/
*.o
*.elf
*.bin
*.hex
*.map
*.lst

# Object files
*.obj
*.exe
*.dll
*.so
*.dylib

# Debug files
*.dSYM/
*.su
*.idb
*.pdb

# IDE files
.vscode/
.idea/
*.swp
*.swo
*~

# OS files
.DS_Store
Thumbs.db

# Dependencies
vendor/
node_modules/

# Logs
*.log
logs/

# Temporary files
*.tmp
*.temp
```

---

## Branching Strategies

### Git Flow Model
```bash
# Git Flow branching model
# Main branches
main                    # Production-ready code
develop                 # Integration branch for features

# Supporting branches
feature/feature-name    # New features
release/version         # Release preparation
hotfix/issue-description # Critical bug fixes

# Branch creation commands
git checkout -b feature/new-feature develop
git checkout -b release/v1.2.0 develop
git checkout -b hotfix/critical-bug main

# Feature branch workflow
git checkout develop
git pull origin develop
git checkout -b feature/user-authentication
# ... make changes ...
git add .
git commit -m "Add user authentication feature"
git push origin feature/user-authentication
# Create pull request to merge into develop
```

### Trunk-Based Development
```bash
# Trunk-based development (simplified workflow)
# Main branch only
main                    # Single main branch

# Short-lived feature branches
feature/quick-feature   # Short-lived feature branches
# ... make changes ...
git add .
git commit -m "Add quick feature"
git push origin feature/quick-feature
# Merge directly to main after review

# Release tags
git tag -a v1.2.0 -m "Release version 1.2.0"
git push origin v1.2.0
```

### Branch Naming Conventions
```bash
# Branch naming patterns
feature/user-auth           # New features
bugfix/login-error          # Bug fixes
hotfix/security-patch       # Critical fixes
release/v1.2.0              # Release preparation
chore/update-dependencies    # Maintenance tasks
docs/api-documentation      # Documentation updates
test/unit-test-coverage     # Testing improvements

# Ticket-based naming
feature/PROJ-123-user-auth  # Feature with ticket number
bugfix/PROJ-456-login-bug   # Bug fix with ticket number
hotfix/PROJ-789-crash-fix   # Hotfix with ticket number
```

---

## Collaborative Workflows

### Pull Request Workflow
```bash
# Pull request workflow
# 1. Create feature branch
git checkout -b feature/new-feature main

# 2. Make changes and commit
git add .
git commit -m "Implement new feature"

# 3. Push branch to remote
git push origin feature/new-feature

# 4. Create pull request on GitHub/GitLab
# - Set target branch (main or develop)
# - Add description and reviewers
# - Link related issues

# 5. Address review feedback
git add .
git commit -m "Address review feedback"
git push origin feature/new-feature

# 6. Merge after approval
# - Squash commits if needed
# - Delete feature branch
```

### Code Review Process
```bash
# Code review checklist
# Pre-review
- [ ] Code compiles without errors
- [ ] All tests pass
- [ ] Code follows style guidelines
- [ ] Documentation is updated
- [ ] No debug code or comments

# Review criteria
- [ ] Code functionality is correct
- [ ] Code is readable and maintainable
- [ ] Error handling is appropriate
- [ ] Performance considerations
- [ ] Security implications
- [ ] Test coverage is adequate

# Post-review
- [ ] Address all review comments
- [ ] Update documentation if needed
- [ ] Re-run tests after changes
- [ ] Get final approval
```

### Conflict Resolution
```bash
# Resolving merge conflicts
# 1. Check conflict status
git status

# 2. Open conflicted files and resolve
# Look for conflict markers:
# <<<<<<< HEAD
# Your changes
# =======
# Their changes
# >>>>>>> branch-name

# 3. Resolve conflicts manually
# Remove conflict markers
# Keep appropriate code

# 4. Stage resolved files
git add <resolved-file>

# 5. Complete merge
git commit -m "Resolve merge conflicts"

# Alternative: Use merge tool
git mergetool
git add .
git commit -m "Resolve conflicts using mergetool"
```

---

## Release Management

### Release Branching Strategy
```bash
# Release branch workflow
# 1. Create release branch from develop
git checkout develop
git pull origin develop
git checkout -b release/v1.2.0

# 2. Version bump and final fixes
# Update version numbers
# Fix any last-minute issues
# Update release notes

# 3. Commit release changes
git add .
git commit -m "Prepare release v1.2.0"

# 4. Merge to main and tag
git checkout main
git merge release/v1.2.0
git tag -a v1.2.0 -m "Release version 1.2.0"

# 5. Merge back to develop
git checkout develop
git merge release/v1.2.0

# 6. Push changes and tags
git push origin main
git push origin develop
git push origin v1.2.0

# 7. Delete release branch
git branch -d release/v1.2.0
git push origin --delete release/v1.2.0
```

### Semantic Versioning
```bash
# Semantic versioning (SemVer)
# Format: MAJOR.MINOR.PATCH
# MAJOR: Incompatible API changes
# MINOR: New functionality (backward compatible)
# PATCH: Bug fixes (backward compatible)

# Version bump examples
1.0.0 -> 1.1.0    # New feature added
1.1.0 -> 1.1.1    # Bug fix
1.1.1 -> 2.0.0    # Breaking change

# Pre-release versions
1.0.0-alpha.1      # Alpha release
1.0.0-beta.1       # Beta release
1.0.0-rc.1         # Release candidate

# Build metadata
1.0.0+build.123    # Build number
1.0.0+20130313144700 # Timestamp
```

### Release Automation
```bash
#!/bin/bash
# release.sh - Automated release script

set -e

# Configuration
VERSION=$1
RELEASE_BRANCH="release/v${VERSION}"
DEVELOP_BRANCH="develop"
MAIN_BRANCH="main"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Functions
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Validate version format
validate_version() {
    if [[ ! $VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        log_error "Invalid version format. Use MAJOR.MINOR.PATCH"
        exit 1
    fi
}

# Check working directory
check_working_directory() {
    if [[ -n $(git status --porcelain) ]]; then
        log_error "Working directory is not clean"
        git status
        exit 1
    fi
}

# Create release branch
create_release_branch() {
    log_info "Creating release branch: $RELEASE_BRANCH"
    
    git checkout $DEVELOP_BRANCH
    git pull origin $DEVELOP_BRANCH
    git checkout -b $RELEASE_BRANCH
    
    log_info "Release branch created successfully"
}

# Update version files
update_version() {
    log_info "Updating version to $VERSION"
    
    # Update version in source files
    sed -i "s/VERSION_MAJOR [0-9]*/VERSION_MAJOR ${VERSION%%.*}/" src/version.h
    sed -i "s/VERSION_MINOR [0-9]*/VERSION_MINOR ${VERSION%.*##*.}/" src/version.h
    sed -i "s/VERSION_PATCH [0-9]*/VERSION_PATCH ${VERSION##*.}/" src/version.h
    
    # Update package.json if exists
    if [[ -f package.json ]]; then
        sed -i "s/\"version\": \"[^\"]*\"/\"version\": \"$VERSION\"/" package.json
    fi
    
    # Update CMakeLists.txt if exists
    if [[ -f CMakeLists.txt ]]; then
        sed -i "s/PROJECT_VERSION [0-9.]*/PROJECT_VERSION $VERSION/" CMakeLists.txt
    fi
    
    log_info "Version files updated"
}

# Commit release changes
commit_release() {
    log_info "Committing release changes"
    
    git add .
    git commit -m "Prepare release v$VERSION"
    
    log_info "Release changes committed"
}

# Merge to main and tag
merge_to_main() {
    log_info "Merging to main branch"
    
    git checkout $MAIN_BRANCH
    git pull origin $MAIN_BRANCH
    git merge $RELEASE_BRANCH --no-ff -m "Merge release v$VERSION"
    
    # Create tag
    git tag -a "v$VERSION" -m "Release version $VERSION"
    
    log_info "Release merged to main and tagged"
}

# Merge back to develop
merge_to_develop() {
    log_info "Merging back to develop"
    
    git checkout $DEVELOP_BRANCH
    git merge $RELEASE_BRANCH --no-ff -m "Merge release v$VERSION back to develop"
    
    log_info "Release merged back to develop"
}

# Push changes
push_changes() {
    log_info "Pushing changes to remote"
    
    git push origin $MAIN_BRANCH
    git push origin $DEVELOP_BRANCH
    git push origin "v$VERSION"
    
    log_info "Changes pushed to remote"
}

# Cleanup
cleanup() {
    log_info "Cleaning up release branch"
    
    git branch -d $RELEASE_BRANCH
    git push origin --delete $RELEASE_BRANCH
    
    log_info "Release branch deleted"
}

# Main release process
main() {
    log_info "Starting release process for version $VERSION"
    
    validate_version
    check_working_directory
    create_release_branch
    update_version
    commit_release
    merge_to_main
    merge_to_develop
    push_changes
    cleanup
    
    log_info "Release v$VERSION completed successfully!"
    log_info "Don't forget to:"
    log_info "1. Create release notes on GitHub/GitLab"
    log_info "2. Notify team members"
    log_info "3. Deploy to production"
}

# Run main function
main "$@"
```

---

## Code Review Process

### Review Guidelines
```markdown
# Code Review Guidelines

## Review Checklist

### Functionality
- [ ] Does the code do what it's supposed to do?
- [ ] Are edge cases handled properly?
- [ ] Is error handling appropriate?
- [ ] Are there any security implications?

### Code Quality
- [ ] Is the code readable and maintainable?
- [ ] Are variable names descriptive?
- [ ] Is the code properly documented?
- [ ] Are there any code smells or anti-patterns?

### Performance
- [ ] Are there any performance issues?
- [ ] Is memory usage appropriate?
- [ ] Are algorithms efficient?
- [ ] Are there any unnecessary operations?

### Testing
- [ ] Are there adequate tests?
- [ ] Do all tests pass?
- [ ] Is test coverage sufficient?
- [ ] Are edge cases tested?

### Documentation
- [ ] Is the code self-documenting?
- [ ] Are complex algorithms explained?
- [ ] Are API changes documented?
- [ ] Are README files updated?

## Review Process

1. **Initial Review**: Check for obvious issues
2. **Detailed Review**: Examine code logic and implementation
3. **Testing**: Verify functionality and test coverage
4. **Documentation**: Ensure documentation is complete
5. **Final Approval**: Approve or request changes
```

### Automated Review Tools
```yaml
# .github/workflows/code-review.yml
name: Code Review

on:
  pull_request:
    types: [opened, synchronize, reopened]

jobs:
  code-quality:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Run static analysis
      run: |
        # Install static analysis tools
        sudo apt update
        sudo apt install cppcheck clang-tidy
        
        # Run Cppcheck
        cppcheck --enable=all --error-exitcode=1 src/
        
        # Run Clang-tidy
        clang-tidy --checks=* src/*.c -- -Iinclude/
    
    - name: Check code formatting
      run: |
        # Check if code follows style guidelines
        find src/ -name "*.c" -exec clang-format --dry-run {} \;
    
    - name: Run unit tests
      run: |
        # Build and run tests
        make test
        
    - name: Check code coverage
      run: |
        # Generate coverage report
        make coverage
        
        # Check minimum coverage threshold
        coverage_threshold=80
        current_coverage=$(cat coverage/coverage.txt | grep "TOTAL" | awk '{print $4}' | sed 's/%//')
        
        if (( $(echo "$current_coverage < $coverage_threshold" | bc -l) )); then
          echo "Code coverage ($current_coverage%) is below threshold ($coverage_threshold%)"
          exit 1
        fi
```

---

## Continuous Integration

### CI/CD Pipeline
```yaml
# .github/workflows/ci-cd.yml
name: CI/CD Pipeline

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main, develop ]

jobs:
  build:
    runs-on: ubuntu-latest
    
    strategy:
      matrix:
        target: [stm32f4, stm32f7, stm32h7]
        config: [debug, release]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install gcc-arm-none-eabi gdb-arm-none-eabi cmake make
    
    - name: Configure build
      run: |
        cmake -B build \
              -DCMAKE_BUILD_TYPE=${{ matrix.config }} \
              -DCURRENT_TARGET=${{ matrix.target }}
    
    - name: Build project
      run: cmake --build build --parallel
    
    - name: Run tests
      run: |
        cd build
        ctest --output-on-failure
    
    - name: Upload artifacts
      uses: actions/upload-artifact@v3
      with:
        name: ${{ matrix.target }}-${{ matrix.config }}
        path: build/*.bin
        retention-days: 30

  deploy:
    needs: build
    runs-on: ubuntu-latest
    if: github.ref == 'refs/heads/main'
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Download artifacts
      uses: actions/download-artifact@v3
      with:
        name: stm32f4-release
    
    - name: Deploy to production
      run: |
        echo "Deploying to production..."
        # Add deployment commands here
```

### Automated Testing
```bash
#!/bin/bash
# run_tests.sh - Automated testing script

set -e

# Configuration
BUILD_DIR="build"
TEST_DIR="tests"
COVERAGE_DIR="coverage"
COVERAGE_THRESHOLD=80

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Functions
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Run unit tests
run_unit_tests() {
    log_info "Running unit tests..."
    
    if [[ -d "$BUILD_DIR" ]]; then
        cd "$BUILD_DIR"
        
        # Run CTest
        if command -v ctest &> /dev/null; then
            ctest --output-on-failure --parallel
            log_info "Unit tests completed"
        else
            log_warning "CTest not available"
        fi
        
        cd ..
    else
        log_error "Build directory not found"
        exit 1
    fi
}

# Run integration tests
run_integration_tests() {
    log_info "Running integration tests..."
    
    if [[ -d "$TEST_DIR/integration" ]]; then
        cd "$TEST_DIR/integration"
        
        # Run integration test scripts
        for test_script in *.sh; do
            if [[ -f "$test_script" ]]; then
                log_info "Running $test_script"
                bash "$test_script"
            fi
        done
        
        cd ../..
        log_info "Integration tests completed"
    else
        log_warning "Integration test directory not found"
    fi
}

# Generate coverage report
generate_coverage() {
    log_info "Generating coverage report..."
    
    if [[ -d "$BUILD_DIR" ]]; then
        cd "$BUILD_DIR"
        
        # Generate coverage data
        if command -v gcov &> /dev/null; then
            make coverage
            
            # Check coverage threshold
            if [[ -f "coverage/coverage.txt" ]]; then
                current_coverage=$(cat coverage/coverage.txt | grep "TOTAL" | awk '{print $4}' | sed 's/%//')
                
                log_info "Current coverage: ${current_coverage}%"
                
                if (( $(echo "$current_coverage < $COVERAGE_THRESHOLD" | bc -l) )); then
                    log_warning "Coverage is below threshold ($COVERAGE_THRESHOLD%)"
                else
                    log_info "Coverage meets threshold requirement"
                fi
            fi
        else
            log_warning "GCOV not available"
        fi
        
        cd ..
    fi
}

# Run static analysis
run_static_analysis() {
    log_info "Running static analysis..."
    
    # Run Cppcheck
    if command -v cppcheck &> /dev/null; then
        cppcheck --enable=all --error-exitcode=1 src/ 2>&1 | tee static_analysis.log
        log_info "Static analysis completed"
    else
        log_warning "Cppcheck not available"
    fi
    
    # Run Clang-tidy
    if command -v clang-tidy &> /dev/null; then
        find src/ -name "*.c" -exec clang-tidy --checks=* {} \; 2>&1 | tee clang_tidy.log
        log_info "Clang-tidy analysis completed"
    else
        log_warning "Clang-tidy not available"
    fi
}

# Main testing process
main() {
    log_info "Starting automated testing process..."
    
    run_unit_tests
    run_integration_tests
    generate_coverage
    run_static_analysis
    
    log_info "All tests completed successfully!"
}

# Run main function
main "$@"
```

---

## Common Issues and Solutions

### Merge Conflicts
```bash
# Resolving complex merge conflicts
# 1. Abort merge if needed
git merge --abort

# 2. Start fresh
git checkout main
git pull origin main
git checkout feature-branch
git rebase main

# 3. Resolve conflicts during rebase
# Edit conflicted files
git add <resolved-file>
git rebase --continue

# 4. Force push if needed (use with caution)
git push origin feature-branch --force-with-lease
```

### Large File Management
```bash
# Git LFS for large files
# Install Git LFS
git lfs install

# Track large file types
git lfs track "*.bin"
git lfs track "*.hex"
git lfs track "*.elf"
git lfs track "*.map"

# Add .gitattributes
git add .gitattributes

# Commit and push
git commit -m "Configure Git LFS for binary files"
git push origin main
```

### Repository Maintenance
```bash
# Clean up repository
# Remove old branches
git remote prune origin
git branch --merged | grep -v "\*" | xargs -n 1 git branch -d

# Clean up tags
git tag -l | xargs git tag -d
git fetch --tags

# Optimize repository
git gc --aggressive --prune=now
git repack -a -d --depth=250 --window=250
```

---

## Best Practices

### 1. **Branch Management**
- Use descriptive branch names
- Keep branches short-lived
- Delete merged branches
- Protect main branches

### 2. **Commit Messages**
- Use clear, descriptive messages
- Follow conventional commit format
- Reference issue numbers
- Keep commits atomic

### 3. **Code Review**
- Review all code changes
- Use automated tools
- Provide constructive feedback
- Maintain review standards

### 4. **Release Management**
- Use semantic versioning
- Automate release process
- Maintain release notes
- Tag all releases

### 5. **Security**
- Never commit secrets
- Use .gitignore properly
- Review access permissions
- Monitor for sensitive data

---

## Interview Questions

### Basic Level
1. **What is version control and why is it important?**
   - Track changes, collaborate, manage versions, rollback

2. **What are the main Git commands?**
   - init, clone, add, commit, push, pull, branch, checkout

3. **How do you resolve merge conflicts?**
   - Edit conflicted files, remove markers, stage, commit

### Intermediate Level
1. **How would you design a branching strategy for a team?**
   - Git Flow, trunk-based development, feature branches

2. **What are the challenges in collaborative development?**
   - Merge conflicts, code review, release coordination

3. **How do you implement continuous integration?**
   - Automated builds, testing, deployment pipelines

### Advanced Level
1. **How would you scale version control for large teams?**
   - Repository organization, access control, automation

2. **What are the security considerations in version control?**
   - Access control, secret management, audit trails

3. **How do you implement deployment automation?**
   - CI/CD pipelines, environment management, rollback strategies

### Practical Coding Questions
1. **Set up a Git repository with branching strategy**
2. **Implement automated release process**
3. **Create CI/CD pipeline configuration**
4. **Design code review workflow**
5. **Implement deployment automation**

---

## Summary
Version control workflows are essential for successful embedded software development. A well-designed workflow provides:

- **Collaboration**: Enable team development and code sharing
- **Quality**: Ensure code review and testing processes
- **Stability**: Maintain stable releases and rollback capability
- **Traceability**: Track all changes and maintain audit trails

The key to successful version control implementation lies in:
- **Clear branching strategies** that fit team size and project needs
- **Automated processes** for building, testing, and deployment
- **Comprehensive code review** with clear guidelines and tools
- **Release management** with proper versioning and automation
- **Security practices** that protect code and sensitive information

By following these principles and implementing the techniques discussed in this guide, development teams can create robust, efficient, and maintainable version control workflows for their embedded projects.
