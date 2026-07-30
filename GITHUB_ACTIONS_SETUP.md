# GitHub Actions & Branch Protection Setup

## Workflow Configuration

The CI/CD pipeline is defined in `.github/workflows/ci.yml`. It:

- **Builds** the project in 3 matrix configurations:
  - Application Debug
  - Application Release
  - Unit Tests (with CTest)
- **Tests** all unit tests via CTest
- **Analyzes** code with Cppcheck
- **Fails** the workflow if any step fails

## Enabling Branch Protection

To block merges when CI fails:

### 1. Go to Repository Settings
- Navigate to your GitHub repo
- Click **Settings** → **Branches**

### 2. Add Branch Protection Rule
- Click **Add rule**
- Branch name pattern: `main` (or `develop`)

### 3. Enable Required Checks
- ✅ Check: **Require status checks to pass before merging**
- ✅ Check: **Require branches to be up to date before merging**
- Select required checks:
  - `build-and-test (Application Debug)`
  - `build-and-test (Application Release)`
  - `build-and-test (Unit Tests)`
  - `status-check`

### 4. Additional Options (Recommended)
- ✅ **Require code reviews before merging** (1+ approvals)
- ✅ **Dismiss stale pull request approvals when new commits are pushed**
- ✅ **Require branches to be up to date before merging**

### 5. Save
- Click **Create** or **Update**

## Workflow Triggers

The pipeline runs on:
- **Push** to `main` or `develop`
- **Pull Requests** against `main` or `develop`

## Running Locally Before Pushing

To test before push:

```bash
# Build Debug Application
cmake --preset=Debug-Application
ninja -C build/Target/application/Debug

# Run Unit Tests
cmake --preset=Debug-UnitTest
ctest --test-dir build/Target/application/unit_test --output-on-failure -V

# Run Cppcheck
cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem \
         --error-exitcode=1 \
         Core/Src/*.c TTA_Protocol/Master/*.c TTA_Protocol/Slave/*.c TTA_Protocol/Common/*.c
```

## Workflow Status Badge

Add to your `README.md`:

```markdown
![CI/CD Pipeline](https://github.com/YOUR_ORG/RTOSP1/actions/workflows/ci.yml/badge.svg)
```

Replace `YOUR_ORG` with your GitHub organization name.
