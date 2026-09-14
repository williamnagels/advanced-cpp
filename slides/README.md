# Building the slides

The slide decks are generated with Marp inside the project Docker image. Run
the commands below from the repository root.

## Build the image

```sh
docker build -t cpptraining -f docker/Dockerfile docker
```

If npm requires a local corporate CA, provide it as a BuildKit secret:

```sh
docker build \
  --secret id=local_ca,src=/path/to/local-ca.crt \
  -t cpptraining \
  -f docker/Dockerfile \
  docker
```

## Generate a PDF

```sh
docker run --rm \
  --user "$(id -u):$(id -g)" \
  --env HOME=/tmp \
  --env TMPDIR=/tmp \
  --mount type=bind,source="$PWD/slides",target=/slides \
  --workdir /slides \
  cpptraining \
  ./generate.sh 2_ranges.md pdf
```

The second argument is the output directory. The generated filename is based
on the Markdown filename, so this command creates `slides/pdf/2_ranges.pdf`.

## Download generated PDFs

The latest slide decks are publicly available from GitHub Pages:

**[View and download the Advanced C++ slides](https://williamnagels.github.io/advanced-cpp/)**

The page is updated automatically after a successful build on the `main`
branch and provides these PDFs:

- `1_intro.pdf`
- `2_ranges.pdf`
- `3_coroutines.pdf`

The GitHub Actions workflow also publishes the same files as the
`advanced-cpp-slides` artifact. To download that ZIP, open the repository on
GitHub, select **Actions**, open a completed **Docker build** run, and find it
under **Artifacts**. GitHub requires you to sign in to download workflow
artifacts.

The Pages links are stable and public. Workflow artifacts are retained for 30
days after each run.